/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>, Sainquake <sainquake@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_RANGESENSOR_TF_MINI_H_
#define SENSORS_RANGESENSOR_TF_MINI_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include "libperiph_common.h"

#define TF_MINI_SERIAL_FRAME_SIZE   9
#define TF_MINI_BUFFER_SIZE         18

typedef struct __attribute__((__packed__)) {
    uint16_t header;
    uint16_t distance;
    uint16_t amp;
    uint16_t temp;
    uint8_t check_sum;
} TfMiniSerialFrame_t;
static_assert(sizeof(TfMiniSerialFrame_t) == TF_MINI_SERIAL_FRAME_SIZE, "Wrong type size");

typedef enum {
    BW_TF_MINI,
    BW_TF_LUNA,
} Benewake_tf_lidar_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @return LIBPERIPH_OK if there is no error, otherwise < 0
 */
int8_t tfMiniInit(Benewake_tf_lidar_t lidar);

/**
 * @brief Parse UART buffer
 * @return true if frame appear, otherwise false
 */
float tfParseRange(const TfMiniSerialFrame_t* frame);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_RANGESENSOR_TF_MINI_H_
