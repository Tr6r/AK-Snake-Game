#ifndef __RTC_DRIVER_H
#define __RTC_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32l1xx_conf.h"
#include <stdlib.h>
#include <stdbool.h>

// #define RTC_INIT_FLAG 0x32F0



  typedef struct {
    int hh;
    int mm;
    int   ss;
} Time;

typedef struct {
    int dd;
    int mm;
    int yy;
    uint8_t dateOfMonth;
    uint8_t monthOfYear;
} Date;

  typedef struct 
  {
    Time time;
    Date date;
  }rtc_time_t;

    void rtc_init_instance(uint32_t period);
    Time rtc_get_time();
    uint32_t rtc_get_time_uint32_sec();
    void rtc_set_time(Time *t);
  void rtc_enable_interrupt(uint32_t period);
void rtc_set_alarm_1hz(void);
void rtc_alarm_irq_init(void);


  /* Khởi tạo RTC (clock, prescaler, interrupt, ...) */

  /* Đọc thời gian hiện tại */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_DRIVER_H */