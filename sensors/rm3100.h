/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file rm3100.h
 * @author d.ponomarev
 */
#ifndef RM3100_H_
#define RM3100_H_

#include <stdint.h>

float sf1xxParseFromI2C(uint8_t i2c_response_buf[2]);

#endif  // RM3100_H_