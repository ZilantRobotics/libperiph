/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ublox_commands.h
 * @author ehsan shaghaei
 * @date Dec 16, 2022
 */

#ifndef SENSORS_GPS_UBLOX_COMMANDS_H
#define SENSORS_GPS_UBLOX_COMMANDS_H

#include <stdint.h>

typedef enum {
    UBX_CMD_FACTORY_RESET,
    UBX_CMD_BAUDRATE_921600,

    UBX_MON_HW,
    UBX_NAV_COV,
    UBX_NAV_PVT,
    UBX_NAV_STATUS,
    UBX_TIM_TIM2,

    UBX_CMD_RATE_10_HZ,
    UBX_CMD_SAVE_CONFIG,
} UbloxCommand;



void ubloxConfigureCommandToDma();

/**
 * @brief configures the ublox interface
 */
int8_t ubloxSendCommand(UbloxCommand command);

#endif  // SENSORS_GPS_UBLOX_COMMANDS_H
