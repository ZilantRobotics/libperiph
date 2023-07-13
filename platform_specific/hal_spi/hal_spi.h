/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef PLATFORM_SPECIFIC_HAL_SPI_H_
#define PLATFORM_SPECIFIC_HAL_SPI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int8_t spiPerformTransaction(uint8_t* tx, uint8_t* rx, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif  // PLATFORM_SPECIFIC_HAL_SPI_H_
