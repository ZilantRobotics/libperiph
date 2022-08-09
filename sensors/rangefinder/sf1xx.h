/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file rangesensor/sf1xx.h
 * @author d.ponomarev
 */
#ifndef RANGESENSOR_SF1XX_H_
#define RANGESENSOR_SF1XX_H_

#include <stdint.h>

float sf1xxParseFromI2C(uint8_t i2c_response_buf[2]);

#endif  // RANGESENSOR_SF1XX_H_