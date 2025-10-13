#include "app_rf24.h"
RF24App_t rf24; // instance thực tế

void RF24App_Init(void)
{
    nRF24_Init();
    uint8_t tx_addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t rx_addr[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};

    memcpy(rf24.txAddr, tx_addr, 5);
    memcpy(rf24.rxAddr, rx_addr, 5);

    rf24.dataRate = nRF24_DataRate_1Mbps;
    rf24.txPower = nRF24_TXPower_0dBm;
    rf24.initialized = true;

    nRF24_TXMode(3, 2, 50, rf24.dataRate, rf24.txPower, nRF24_CRC_2byte, nRF24_PWR_Up, rf24.txAddr, 5);
    nRF24_RXMode(nRF24_RX_PIPE0, nRF24_ENAA_P0, 50, rf24.dataRate, nRF24_CRC_2byte, rf24.rxAddr, 5, 32, rf24.txPower);
}

nRF24_TX_PCKT_TypeDef RF24App_Send(uint8_t *data, uint8_t len)
{
    if (!rf24.initialized)
        return nRF24_TX_ERROR;
    return nRF24_TXPacket(data, len);
}

nRF24_RX_PCKT_TypeDef RF24App_Receive(uint8_t *data, uint8_t len)
{
    if (!rf24.initialized)
        return nRF24_RX_PCKT_ERROR;
    return nRF24_RXPacket(data, len);
}