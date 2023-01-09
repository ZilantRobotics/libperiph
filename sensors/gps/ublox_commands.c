/***
 * @file ublox_commands.c
 * @author ehsan shaghaei
 * @date Dec 21, 2022
 */

#include "ublox_commands.h"
#include "libperiph_common.h"
#include "hal_uart.h"

/***
 *  @brief: uBlox configuration: Restore the factory configuration
 *
 *  PS. each uBlox Package starts with two bytes 0xB5, 0x62  and ends with checksum
 *      refer to user manual.
 */
static uint8_t uBloxConfigFactoryReset[] = {
    0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x1B,
    0x9A};
/***
 * @brief: sets the Port configuration as following:
 * Target : UART 1
 * Protocol in : 0+1+2-UBX+NMEA+RTCM
 * Protocol out : 0+1-UBX+NMEA
 * BaudRate: 1152000
 */
static uint8_t uBloxPortProfile0[] = {
    0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00,
    0xD0, 0x08, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x07, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x7E};
/***
 *  @brief: polls for current Port configuration on uBlox Module
 */
static uint8_t uBloxPortPoll[] = {
    0xB5, 0x62, 0x06, 0x00, 0x01, 0x00, 0x01, 0x08, 0x22};

/***
 *  @brief: configures the GPS rate to 10Hz
 *  PS. this command contains two pakcages RATE+CFG
 *
 */
static uint8_t uBloxRates10Hz[] = {
    0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x7A, 0x12, 0xB5, 0x62, 0x06, 0x08, 0x00, 0x00,
    0x0E, 0x30};

/***
 *  @brief: configures Messages 01-07 NAV-PVT on UART 1
 *  PS. this command contains two pakcages MSG+CFG
 */
static uint8_t uBloxMessagesNavPvtEnable[] = {
    0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x07, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x18, 0xE1, 0xB5, 0x62, 0x06, 0x01,
    0x02, 0x00, 0x01, 0x07, 0x11, 0x3A};

/***
 *  @brief: deconfigures Messages 01-07 NAV-PVT on UART 1
 *  PS. this command contains two pakcages MSG+CFG
 */
static uint8_t uBloxMessagesNavPvtDisable[] = {
    0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x07, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x17, 0xDC, 0xB5, 0x62, 0x06, 0x01,
    0x02, 0x00, 0x01, 0x07, 0x11, 0x3A};

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
