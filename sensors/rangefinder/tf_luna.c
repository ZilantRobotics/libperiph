/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file tf_luna.c
 * @author d.ponomarev
 * @note https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf
 */

#include "tf_luna.h"
#include <string.h>

#define HEADER_BYTE 0x59

typedef enum {
    STATE_SYNC_CHAR_1,
    STATE_SYNC_CHAR_2,
    STATE_DIST_L,
    STATE_DIST_H,
    STATE_AMP_L,
    STATE_AMP_H,
    STATE_TEMP_L,
    STATE_TEMP_H,
    STATE_CHECK_SUM,
} TfLunaState_t;

typedef struct {
    uint16_t header;
    uint16_t distance;
    uint16_t amp;
    uint16_t temp;
    uint8_t check_sum;
} TfLunaSerialFrame_t;


static bool tfLunaNextByte(uint8_t byte);
static bool tfLunaParseSerialFrame();
static uint8_t crc_8(const uint8_t* buf, uint8_t size);

// This buffer should have previous frame and current!
static uint8_t buffer[TF_LUNA_BUFFER_SIZE];
static TfLunaState_t state;


int8_t tfLunaInit() {
    return 0;
}

bool tfLunaCollectData(const uint8_t* buffer_ptr) {
    if (!buffer_ptr) {
        return false;
    }

    // copy previous frame to the beginning
    memcpy(buffer, buffer + TF_LUNA_SERIAL_FRAME_SIZE, TF_LUNA_SERIAL_FRAME_SIZE);

    // save the new frame
    memcpy(buffer + TF_LUNA_SERIAL_FRAME_SIZE, buffer_ptr, TF_LUNA_SERIAL_FRAME_SIZE);
    return tfLunaParseSerialFrame();
}

float tfLunaParseCollectedData() {
    TfLunaSerialFrame_t* frame = (TfLunaSerialFrame_t*)&buffer[TF_LUNA_SERIAL_FRAME_SIZE];
    uint16_t distance_sm = frame->distance;
    return distance_sm * 0.01;
}

bool tfLunaParseSerialFrame() {
    for (uint32_t idx = TF_LUNA_SERIAL_FRAME_SIZE; idx < TF_LUNA_BUFFER_SIZE; idx++) {
        if (tfLunaNextByte(buffer[idx])) {
            return crc_8(buffer + idx - 8, 8) == buffer[idx];
        }
    }
    return false;
}

bool tfLunaNextByte(uint8_t byte) {
    switch (state) {
        case STATE_SYNC_CHAR_1:
            if (byte == HEADER_BYTE) {
                state = STATE_SYNC_CHAR_2;
            }
            break;
        case STATE_SYNC_CHAR_2:
            state = (byte == HEADER_BYTE) ? STATE_DIST_L : STATE_SYNC_CHAR_1;
            break;
        case STATE_DIST_L:
            state = (byte != HEADER_BYTE) ? STATE_DIST_H : STATE_SYNC_CHAR_1;
            break;
        case STATE_DIST_H:
            state = (byte != HEADER_BYTE) ? STATE_AMP_L : STATE_SYNC_CHAR_1;
            break;
        case STATE_AMP_L:
            state = (byte != HEADER_BYTE) ? STATE_AMP_H : STATE_SYNC_CHAR_1;
            break;
        case STATE_AMP_H:
            state = (byte != HEADER_BYTE) ? STATE_TEMP_L : STATE_SYNC_CHAR_1;
            break;
        case STATE_TEMP_L:
            state = (byte != HEADER_BYTE) ? STATE_TEMP_H : STATE_SYNC_CHAR_1;
            break;
        case STATE_TEMP_H:
            state = (byte != HEADER_BYTE) ? STATE_CHECK_SUM : STATE_SYNC_CHAR_1;
            break;
        case STATE_CHECK_SUM:
            state = STATE_SYNC_CHAR_1;
            return true;
        default:
            break;
    }
    return false;
}

uint8_t crc_8(const uint8_t* buf, uint8_t size) {
    uint8_t sum = 0;
    for (uint_fast8_t idx = 0; idx < size; idx++) {
        sum += buf[idx];
    }
    return sum;
}
