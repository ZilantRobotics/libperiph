/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef PLATFORM_SPECIFIC_HAL_I2C_H_
#define PLATFORM_SPECIFIC_HAL_I2C_H_

#include <stdint.h>

/**
 * @brief Simple wrappers under HAL TX/RX functions
 */
int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len);
int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len);

#endif  // PLATFORM_SPECIFIC_HAL_I2C_H_
