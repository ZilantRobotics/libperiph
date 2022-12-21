/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file tf_luna.h
 * @author d.ponomarev
 * @note https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf
 */
#ifndef SENSORS_RANGESENSOR_TF_LUNA_H_
#define SENSORS_RANGESENSOR_TF_LUNA_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#define TF_LUNA_SERIAL_FRAME_SIZE   9
#define TF_LUNA_BUFFER_SIZE         18

typedef struct __attribute__((__packed__)) {
    uint16_t header;
    uint16_t distance;
    uint16_t amp;
    uint16_t temp;
    uint8_t check_sum;
} TfLunaSerialFrame_t;
static_assert(sizeof(TfLunaSerialFrame_t) == TF_LUNA_SERIAL_FRAME_SIZE, "Wrong type size");

/**
 * @return STATUS_OK if there is no error, otherwise STATUS_ERROR
 */
int8_t tfLunaInit();


/**
 * @brief Parse UART buffer
 * @return true if frame appear, otherwise false
 */
float tfParseRange(const TfLunaSerialFrame_t* frame);

#endif  // SENSORS_RANGESENSOR_TF_LUNA_H_
