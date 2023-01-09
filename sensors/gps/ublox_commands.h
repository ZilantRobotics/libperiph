/**
 * @file ublox.h
 * @author ehsan shaghaei
 * @date Dec 16, 2022
 * @brief This header file contains uBlox  commands.
 *  the naming snakeCase convention is as follows:
 * - static  uint8_t <Protocol><PacketType><ContentProfile> [] ={bytes,...};
 *  e.g,
 *     static  uint8_t uBloxPortPoll[] = {
        0xB5, 0x62, 0x06, 0x00, 0x01, 0x00, 0x01, 0x08, 0x22};
        * protocol -> uBlox
        * PacketType -> Port
        * ContentProfile -> Poll

 * 
 */
#ifndef __UBLOX_COMMANDS_H
#define __UBLOX_COMMANDS_H

#include <stdint.h>
#include <stdbool.h>



uint8_t ubloxStartup(bool DMA);

#endif // __UBLOX_H