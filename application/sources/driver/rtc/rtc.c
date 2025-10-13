#include "rtc.h"
#include "ak.h"
rtc_time_t rtc_datetime;
#define RTC_INIT_FLAG 0x32F2

void init(void)
{
    RTC_InitTypeDef RTC_InitStruct;

    /* Enable power clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    PWR_RTCAccessCmd(ENABLE);

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        ;

    /* Select LSE as RTC clock source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();

    /* Configure RTC prescaler for 1Hz */
    RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStruct.RTC_AsynchPrediv = 0x7F; // 127
    RTC_InitStruct.RTC_SynchPrediv = 0xFF;  // 255

    RTC_Init(&RTC_InitStruct);
    RTC_WaitForSynchro();

    /* Set backup register flag to indicate RTC initialized */
}

void rtc_init_instance(uint32_t period)
{
    init(); // Init cơ bản RTC/LSE

    uint32_t val = RTC_ReadBackupRegister(RTC_BKP_DR0);
    xprintf("\r\n[RTC DEBUG]");
    xprintf("\r\n  BKP0R (val)        = %lu (0x%08lX)", val, val);
    xprintf("\r\n  RTC_INIT_FLAG      = %lu (0x%08lX)\r\n",
            (uint32_t)RTC_INIT_FLAG, (uint32_t)RTC_INIT_FLAG);
    if (val != RTC_INIT_FLAG)
    {
        RTC_WriteProtectionCmd(DISABLE);

        RTC_TimeTypeDef rtcTime;
        RTC_DateTypeDef rtcDate;

        rtcTime.RTC_Hours = 3;
        rtcTime.RTC_Minutes = 3;
        rtcTime.RTC_Seconds = 3;

        rtcDate.RTC_Date = 1;  // ngày bắt đầu = 1
        rtcDate.RTC_Month = 1; // tháng bắt đầu = 1
        rtcDate.RTC_Year = 0;

        RTC_SetTime(RTC_Format_BIN, &rtcTime);
        RTC_SetDate(RTC_Format_BIN, &rtcDate);

        /* Ghi flag lần đầu */
        RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_INIT_FLAG);

        RTC_WriteProtectionCmd(ENABLE);
    }
    // else if (val == 0x0000)
    // {
    //     RTC_WriteProtectionCmd(DISABLE);

    //     RTC_TimeTypeDef rtcTime;
    //     RTC_DateTypeDef rtcDate;

    //     rtcTime.RTC_Hours = 1;
    //     rtcTime.RTC_Minutes = 1;
    //     rtcTime.RTC_Seconds = 1;

    //     rtcDate.RTC_Date = 1;  // ngày bắt đầu = 1
    //     rtcDate.RTC_Month = 1; // tháng bắt đầu = 1
    //     rtcDate.RTC_Year = 0;

    //     RTC_SetTime(RTC_Format_BIN, &rtcTime);
    //     RTC_SetDate(RTC_Format_BIN, &rtcDate);

    //     /* Ghi flag lần đầu */
    //     RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_INIT_FLAG);

    //     RTC_WriteProtectionCmd(ENABLE);
    // }
     
}

/* Hàm enable RTC WakeUp timer 1Hz (hoặc period tuỳ chỉnh) */
void rtc_enable_interrupt(uint32_t period)
{
    // 1. Unlock RTC registers
    RTC_WriteProtectionCmd(DISABLE);

    // 2. Chọn clock cho WakeUp timer
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);

    // 3. Set counter, luôn >= 1
    if (period == 0)
        period = 1;
    RTC_SetWakeUpCounter(period - 1);

    // 4. Bật interrupt
    RTC_ITConfig(RTC_IT_WUT, ENABLE);
    RTC_WakeUpCmd(ENABLE);

    // 5. Enable EXTI line 20
    EXTI_ClearITPendingBit(EXTI_Line20);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 6. Enable NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 7. Lock RTC registers
    RTC_WriteProtectionCmd(ENABLE);
}

void rtc_set_alarm_1hz(void)
{
    RTC_AlarmTypeDef RTC_AlarmStruct;

    RTC_WriteProtectionCmd(DISABLE);

    // Đặt Alarm A: báo mỗi giây
    RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours = 0;
    RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0;
    RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0;
    RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_Hours |
                                    RTC_AlarmMask_Minutes |
                                    RTC_AlarmMask_DateWeekDay;
    // chỉ so giây thôi → nhảy mỗi giây
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStruct);
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

    RTC_ITConfig(RTC_IT_ALRA, ENABLE); // enable interrupt Alarm A

    RTC_WriteProtectionCmd(ENABLE);
}

void rtc_alarm_irq_init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // EXTI line 17 dùng cho RTC Alarm
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

Time rtc_get_time()
{
    RTC_TimeTypeDef hwTime;
    RTC_GetTime(RTC_Format_BIN, &hwTime);
    rtc_datetime.time.hh = hwTime.RTC_Hours;
    rtc_datetime.time.mm = hwTime.RTC_Minutes;
    rtc_datetime.time.ss = hwTime.RTC_Seconds;

    return rtc_datetime.time;
}
uint32_t rtc_get_time_uint32_sec()
{
    RTC_TimeTypeDef hwTime;
    RTC_GetTime(RTC_Format_BIN, &hwTime);
    return hwTime.RTC_Hours * 3600 + hwTime.RTC_Minutes * 60 + hwTime.RTC_Seconds;
}

void rtc_set_time(Time *t)
{

    RTC_TimeTypeDef rtcTime;

    rtcTime.RTC_Hours = t->hh;
    rtcTime.RTC_Minutes = t->mm;
    rtcTime.RTC_Seconds = t->ss;

    PWR_RTCAccessCmd(ENABLE);
    RTC_WriteProtectionCmd(DISABLE);

    RTC_SetTime(RTC_Format_BIN, &rtcTime);

    RTC_WriteProtectionCmd(ENABLE);
}
