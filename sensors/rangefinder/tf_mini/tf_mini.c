/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>, sainquake <sainquake@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "tf_mini.h"
#include <string.h>
#include "libperiph_common.h"

#define TF_LUNA_HEAD_BYTE 0x5A
#define TF_MINI_HEAD_BYTE 0x59

static int8_t tfMiniFindFrameStart();
static float tfMiniParseData(int8_t idx);
static uint8_t crc_8(const uint8_t* buf, uint8_t size);

// This buffer should have previous frame and current!
static uint8_t buffer[TF_MINI_BUFFER_SIZE];

// TF_MINI by default
static uint8_t head_byte = TF_MINI_HEAD_BYTE;

int8_t tfMiniInit(Benewake_tf_lidar_t lidar) {
    if (lidar == TF_LUNA_HEAD_BYTE) {
        head_byte = TF_LUNA_HEAD_BYTE;
    } else {
        head_byte = TF_MINI_HEAD_BYTE;
    }
    return LIBPERIPH_OK;
}

float tfParseRange(const TfMiniSerialFrame_t* buffer_ptr) {
    if (!buffer_ptr) {
        return -1.0;
    }

    // copy previous frame to the beginning and save the new one
    memcpy(buffer, buffer + TF_MINI_SERIAL_FRAME_SIZE, TF_MINI_SERIAL_FRAME_SIZE);
    memcpy(buffer + TF_MINI_SERIAL_FRAME_SIZE, buffer_ptr, TF_MINI_SERIAL_FRAME_SIZE);

    int8_t idx = tfMiniFindFrameStart();
    if (idx >= 0) {
        return tfMiniParseData(idx);
    }
    return -1.0;
}

///< *************************** PRIVATE FUNCTIONS ****************************

static int8_t tfMiniFindFrameStart() {
    for (int8_t idx = TF_MINI_SERIAL_FRAME_SIZE; idx > 0; idx--) {
        if (buffer[idx] == HEAD_BYTE && buffer[idx + 1] == HEAD_BYTE && crc_8(buffer + idx, 8)) {
            return idx;
        }
    }
    return LIBPERIPH_ERROR;
}

static float tfMiniParseData(int8_t idx) {
    const TfMiniSerialFrame_t* frame = (const TfMiniSerialFrame_t*)&buffer[idx];
    uint16_t distance_sm = frame->distance;
    return distance_sm * 0.01f;
}

static uint8_t crc_8(const uint8_t* buf, uint8_t size) {
    uint8_t sum = 0;
    for (uint_fast8_t idx = 0; idx < size; idx++) {
        sum += buf[idx];
    }
    return sum;
}
