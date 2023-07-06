/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_ESC_COMMON_H_
#define SENSORS_ESC_COMMON_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t* buf;
    size_t size;
    size_t saved_idx;
} DmaUartHandler_t;

#endif  // SENSORS_ESC_COMMON_H_
