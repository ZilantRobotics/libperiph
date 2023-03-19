/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ublox.h
 * @author ehsan shaghaei
 * @date Dec 16, 2022
 * @brief This header file contains uBlox  commands.
 */

#ifndef SENSORS_GPS_UBLOX_COMMANDS_H
#define SENSORS_GPS_UBLOX_COMMANDS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief configures the ublox interface
 * @param DMA boolean flag to send configuration by DMA or Polling mode
 */
uint8_t ubloxConfigure(bool DMA);

#endif  // SENSORS_GPS_UBLOX_COMMANDS_H
