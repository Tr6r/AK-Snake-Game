#ifndef __APP_RF24_H__
#define __APP_RF24_H__

#include <string.h>
#include "nRF24.h"


enum {
    RF24_SIG_RX_READY,
    RF24_SIG_TX_DONE,
    RF24_SIG_TX_FAIL,
};

// Struct quản lý RF24
typedef struct {
    uint8_t txAddr[5];
    uint8_t rxAddr[5];
    uint8_t payload[32];
    nRF24_DataRate_TypeDef dataRate;
    nRF24_TXPower_TypeDef txPower;
    bool initialized;
} RF24App_t;

extern RF24App_t rf24;

// Interface của module
extern void RF24App_Init(void);
extern nRF24_TX_PCKT_TypeDef RF24App_Send(uint8_t *data, uint8_t len);
extern nRF24_RX_PCKT_TypeDef RF24App_Receive(uint8_t *data, uint8_t len);


#endif //__APP_RF24_H__
