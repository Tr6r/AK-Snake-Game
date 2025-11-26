#ifndef __TASK_UART_FWU_H__
#define __TASK_UART_FWU_H__

/* uart_boot_frame_t */
#define UART_BOOT_FRAME_DATA_SIZE			(254)
#define UART_BOOT_CMD_DATA_SIZE				(128)

#define UART_BOOT_SOP_CHAR		0xEF

/* boot frame parser */
#define SOP_STATE		0x00
#define LEN_STATE		0x01
#define DATA_STATE		0x02
#define FCS_STATE		0x03


/* ACK command */
#define UART_FWU_READY			(0x03)
#define UART_FWU_NACK			(0x04)
#define UART_FWU_ACK			(0x06)


typedef enum {
	UART_BOOT_TYPE_META,
	UART_BOOT_TYPE_CHUNK,
	UART_BOOT_TYPE_FCS,
} uart_boot_frame_type_t;

typedef struct {
    uart_boot_frame_type_t type;
	uint8_t state;
	uint8_t sop;
	uint8_t len;
	uint8_t index;
	uint8_t data[UART_BOOT_FRAME_DATA_SIZE];
	uint8_t fcs;
} uart_boot_frame_t;

extern void uart_rx_frame_parser(uint8_t ch);
extern void uarrt_tx_ack(void);
extern void uarrt_tx_nack(void);
extern void uarrt_tx_ready(void);

extern void uart_boot_init(void);
#endif //__TASK_UART_FWU_H__
