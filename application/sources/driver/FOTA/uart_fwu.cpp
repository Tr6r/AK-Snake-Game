#include <stdbool.h>

#include "fsm.h"
#include "port.h"
#include "message.h"

#include "cmd_line.h"
#include "xprintf.h"

#include "sys_dbg.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "sys_io.h"

#include "app.h"
#include "app_dbg.h"
#include "uart_fwu.h"
#include "task_list.h"
#include "task_fw.h"

static uart_boot_frame_t uart_boot_frame;

void uarrt_tx_ack(void);
void uarrt_tx_nack(void);
void uarrt_tx_ready(void);
static uint8_t uart_boot_calcfcs(uint8_t len, uint8_t *data_ptr);
void uart_boot_init(void);

void uart_rx_frame_parser(uint8_t ch)
{
    switch (uart_boot_frame.state)
    {
    case SOP_STATE:
    {
        if (UART_BOOT_SOP_CHAR == ch)
        {
            uart_boot_frame.state = LEN_STATE;
            uarrt_tx_ack();
            return;
        }
        uarrt_tx_nack();
    }
    break;

    case LEN_STATE:
    {
        if (ch > UART_BOOT_FRAME_DATA_SIZE)
        {
            uart_boot_frame.state = SOP_STATE;
            uarrt_tx_nack();
            return;
        }
        else
        {
            uart_boot_frame.len = ch;
            uart_boot_frame.index = 0;
            uart_boot_frame.state = DATA_STATE;
            uarrt_tx_ack();
        }
    }
    break;

    case DATA_STATE:
    {
        uart_boot_frame.data[uart_boot_frame.index++] = ch;
        // xprintf("DATA RX INDEX: %d/%d\r\n", uart_boot_frame.index, uart_boot_frame.len);
        if (uart_boot_frame.index == uart_boot_frame.len)
        {
            uart_boot_frame.state = FCS_STATE;
            APP_DBG("[MCU] Rx chunk\r\n");
            uarrt_tx_ack();
            return;
        }
            // uarrt_tx_ack();

        
    }
    break;

    case FCS_STATE:
    {
        uart_boot_frame.state = SOP_STATE;

        uart_boot_frame.fcs = ch;

        if (uart_boot_frame.fcs == uart_boot_calcfcs(uart_boot_frame.len, uart_boot_frame.data))
        {
            switch (uart_boot_frame.type)
            {
            case UART_BOOT_TYPE_META:
                task_post_common_msg(AC_TASK_FW_ID, FW_UPDATE_REQ, uart_boot_frame.data, uart_boot_frame.len);
                uart_boot_frame.type = UART_BOOT_TYPE_CHUNK;
                break;
            case UART_BOOT_TYPE_CHUNK:
                task_post_common_msg(AC_TASK_FW_ID, FW_TRANSFER_REQ, uart_boot_frame.data, uart_boot_frame.len);

                break;
            case UART_BOOT_TYPE_FCS:

                break;

            default:
                break;
            }
        }
        else
        {
            uarrt_tx_nack();
        }
    }
    break;

    default:
        break;
    }
}
void uart_boot_init()
{
    uart_boot_frame.type = UART_BOOT_TYPE_META;
    uart_boot_frame.index = 0;
    uart_boot_frame.len = 0;
    uart_boot_frame.state = SOP_STATE;
}

void uarrt_tx_ack()
{
    uint8_t ack = UART_FWU_ACK;
    xprintf("%c", ack);
}
void uarrt_tx_nack()
{
    uint8_t nack = UART_FWU_NACK;
    xprintf("%c", nack);
}
void uarrt_tx_ready()
{
    uint8_t ready = UART_FWU_READY;
    xprintf("%c", ready);
}
uint8_t uart_boot_calcfcs(uint8_t len, uint8_t *data_ptr)
{
    uint8_t xor_result;
    xor_result = len;
    for (int i = 0; i < len; i++, data_ptr++)
    {
        xor_result = xor_result ^ *data_ptr;
    }
    return xor_result;
}