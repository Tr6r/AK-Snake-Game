#include <stdbool.h>

#include "fsm.h"
#include "port.h"
#include "message.h"

#include "xprintf.h"

#include "sys_dbg.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "sys_io.h"

#include "app.h"
#include "app_dbg.h"
#include "task_log.h"
#include "task_list.h"

ring_log_buffer_t ring_buffer_log;
uint8_t buffer_log[BUFFER_LOG_SIZE];

int count_test = 1;

void task_log(ak_msg_t *msg)
{
    switch (msg->sig)
    {

    case AC_LOG_PRINT:
    {
        ring_buffer_print_logs(&ring_buffer_log);
    }
    break;

    case AC_LOG_PUT:
    {
        log_entry_t temp;
        temp.header.magic = 0x11;
        uint32_t t = rtc_get_time_uint32_sec();
        uint8_t hh = t / 3600;
        uint8_t mm = (t % 3600) / 60;
        uint8_t ss = t % 60;
        xprintf("%02u:%02u:%02u\n", hh, mm, ss);
        temp.header.timestamp = t;
        temp.header.data_type = 1;
        temp.header.len = 1;
        temp.data[0] = count_test++;

        ring_buffer_put_log(&ring_buffer_log, &temp);
    }

    break;

    case AC_LOG_PUT_EEPROM:
    {
        log_buffer_to_eeprom(&ring_buffer_log);
    }
    break;
     case AC_LOG_PRINT_EEPROM:
    {
        print_logs_from_eeprom(&ring_buffer_log);
    }
    break;
case AC_CLEAR_LOG_EEPROM:
    {
        log_clear_eeprom();
    }
    break;
    default:
        break;
    }
}