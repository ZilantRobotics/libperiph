/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ublox_emulation.h
 * @author d.ponomarev
 */

#ifndef GPS_UBLOX_EMULATION_H_
#define GPS_UBLOX_EMULATION_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "ublox.h"
#include "ublox_internal.h"


/**
 * @brief Ublox emulation
 */
void ubloxCreateRawDataFromPackage(UbxNavPvtRaw_t* buffer, const GnssUblox_t* uavcan_fix2);

#endif  // GPS_UBLOX_EMULATION_H_