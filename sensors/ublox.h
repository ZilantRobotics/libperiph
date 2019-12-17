/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file gps_ublox.h
 * @author d.ponomarev
 */

#ifndef UBLOX_H_
#define UBLOX_H_

#include <stdint.h>
#include <stddef.h>

void ubloxNextByte(uint8_t byte);

#endif  // __UBLOX_H_