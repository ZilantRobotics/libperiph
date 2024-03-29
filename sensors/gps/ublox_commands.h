/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_GPS_UBLOX_COMMANDS_H
#define SENSORS_GPS_UBLOX_COMMANDS_H

#include <stdint.h>
#include <stddef.h>
#include "libperiph_common.h"

#ifdef __cplusplus
typedef enum : uint8_t {
#else
typedef enum {
#endif
    UBX_CMD_FACTORY_RESET,
    UBX_CMD_BAUDRATE_921600,

    NODE_SET_BAUDRATE_9600,
    NODE_SET_BAUDRATE_38400,
    NODE_SET_BAUDRATE_115200,
    NODE_SET_BAUDRATE_921600,

    UBX_CFG_NAV_5,
    UBX_CFG_TP_5,

    UBX_CMD_DISABLE_GGA,
    UBX_CMD_DISABLE_GLL,
    UBX_CMD_DISABLE_GSA,
    UBX_CMD_DISABLE_GSV,
    UBX_CMD_DISABLE_RMC,
    UBX_CMD_DISABLE_VTG,

    UBX_MON_HW,
    UBX_NAV_COV,
    UBX_NAV_PVT,
    UBX_NAV_STATUS,
    UBX_TIM_TIM2,

    UBX_CMD_RATE_10_HZ,
    UBX_CMD_SAVE_CONFIG,

    UBX_UNKNOWN_COMMAND,
} UbloxCommand;

#ifdef __cplusplus
extern "C" {
#endif

typedef int8_t (*UbxTransmit_t)(const uint8_t[], size_t);
typedef void (*UbxDelay_t)(uint32_t);
typedef void (*UbxChangeBaudRate_t)(uint32_t);


/**
 * @brief Initialize the module. You have to provide 3 functions pointers.
 * @param transmit - either DMA or not
 * @param delay - either os related or blocking
 * @param changeBaudRate - required to be able to configure the gnss on any baudrate
 */
int8_t ubloxInit(UbxTransmit_t transmit, UbxDelay_t delay, UbxChangeBaudRate_t changeBaudRate);

/**
 * @brief Configure U-Blox gnss receiver
 */
int8_t ubloxConfigure(uint16_t delay);


/**
 * @brief Execute a specific command
 */
int8_t ubloxExecuteCommand(UbloxCommand command);

UbloxCommand ubloxGetCommand(uint8_t idx);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_GPS_UBLOX_COMMANDS_H
