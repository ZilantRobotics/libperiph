/**
 * @file ublox.h
 * @author ehsan shaghaei
 * @date Dec 16, 2022
 * @brief This header file contains uBlox  commands.
 */
#ifndef __UBLOX_COMMANDS_H
#define __UBLOX_COMMANDS_H

#include <stdint.h>
#include <stdbool.h>



uint8_t ubloxStartup(bool DMA);
uint8_t ubloxConfigure(bool DMA);
#endif // __UBLOX_H