/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SRC_HAL_STM32_HAL_EXTI_H_
#define SRC_HAL_STM32_HAL_EXTI_H_


#include <stdint.h>
#include <stdbool.h>


int8_t extiInit();
bool extiIsInited();
bool extiWaitForInterrupt_12_15(uint16_t block_time_ms);
bool extiWaitForInterrupt_0_11(uint16_t block_time_ms);


#endif  // SRC_HAL_STM32_HAL_EXTI_H_
