/***
 * @file ublox_commands.c
 * @author ehsan shaghaei
 * @date Dec 21, 2022
 */

#include "ublox_commands.h"
#include "libperiph_common.h"
#include "hal_uart.h"

/**
 * @brief configures the ublox interface
 * @param DMA boolean flag to send configuration by DMA or Polling mode
 *
 */
uint8_t ubloxConfigure(bool DMA)
{

    int8_t (*Transmit)(uint8_t[], size_t) = DMA ? &uartTransmitDma : &uartTransmit;

    UartChangeBaudrate(115200);
    if (Transmit(uBloxConfigFactoryReset,
                  sizeof(uBloxConfigFactoryReset) / sizeof(*uBloxConfigFactoryReset)) == STATUS_ERROR)
    {
        return STATUS_ERROR;
    };

    UartChangeBaudrate(9600);
    if (Transmit(uBloxConfigFactoryReset,
                  sizeof(uBloxConfigFactoryReset) / sizeof(*uBloxConfigFactoryReset)) == STATUS_ERROR)
    {
        return STATUS_ERROR;
    };
    if (Transmit(uBloxPortProfile0,
                  sizeof(uBloxPortProfile0) / sizeof(*uBloxPortProfile0)) == STATUS_ERROR)
    {
        return STATUS_ERROR;
    };

    UartChangeBaudrate(115200);
    if (Transmit(uBloxMessagesNavPvtDisable,
                  sizeof(uBloxMessagesNavPvtDisable) / sizeof(*uBloxMessagesNavPvtDisable)) == STATUS_ERROR)
    {
        return STATUS_ERROR;
    };

    if (Transmit(uBloxRates10Hz,
                  sizeof(uBloxRates10Hz) / sizeof(*uBloxRates10Hz)) == STATUS_ERROR)
    {
        return STATUS_ERROR;
    };


    if (Transmit(uBloxMessagesNavPvtEnable,
                  sizeof(uBloxMessagesNavPvtEnable) / sizeof(*uBloxMessagesNavPvtEnable)) == STATUS_ERROR)
    {
        return STATUS_ERROR;
    };

    return STATUS_OK;
}
