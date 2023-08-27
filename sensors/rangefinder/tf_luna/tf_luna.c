/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "tf_luna.h"
#include <string.h>
#include "libperiph_common.h"

#define HEAD_BYTE 0x59


static int8_t tfLunaFindFrameStart();
static float tfLunaParseData(int8_t idx);
static uint8_t crc_8(const uint8_t* buf, uint8_t size);

// This buffer should have previous frame and current!
static uint8_t buffer[TF_LUNA_BUFFER_SIZE];


int8_t tfLunaInit() {
    return LIBPERIPH_OK;
}

float tfParseRange(const TfLunaSerialFrame_t* buffer_ptr) {
    if (!buffer_ptr) {
        return -1.0;
    }

    // copy previous frame to the beginning and save the new one
    memcpy(buffer, buffer + TF_LUNA_SERIAL_FRAME_SIZE, TF_LUNA_SERIAL_FRAME_SIZE);
    memcpy(buffer + TF_LUNA_SERIAL_FRAME_SIZE, buffer_ptr, TF_LUNA_SERIAL_FRAME_SIZE);

    int8_t idx = tfLunaFindFrameStart();
    if (idx >= 0) {
        return tfLunaParseData(idx);
    }
    return -1.0;
}

int8_t tfLunaFindFrameStart() {
    int8_t idx;
    for (idx = TF_LUNA_SERIAL_FRAME_SIZE; idx > 0; idx--) {
        if (buffer[idx] == HEAD_BYTE && buffer[idx + 1] == HEAD_BYTE && crc_8(buffer + idx, 8)) {
            return idx;
        }
    }
    return LIBPERIPH_ERROR;
}

float tfLunaParseData(int8_t idx) {
    TfLunaSerialFrame_t* frame = (TfLunaSerialFrame_t*)&buffer[idx];
    uint16_t distance_sm = frame->distance;
    return distance_sm * 0.01f;
}

uint8_t crc_8(const uint8_t* buf, uint8_t size) {
    uint8_t sum = 0;
    for (uint_fast8_t idx = 0; idx < size; idx++) {
        sum += buf[idx];
    }
    return sum;
}
