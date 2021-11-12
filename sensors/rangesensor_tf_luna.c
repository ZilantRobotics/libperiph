/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file rangesensor_tf_luna.c
 * @author d.ponomarev
 * @note https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf
 */

#include "rangesensor_tf_luna.h"
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


static uint8_t uart_buf[TF_LUNA_BUFFER_SIZE];           ///< should have double frame size
static uint8_t frame_buf[TF_LUNA_SERIAL_FRAME_SIZE];
static uint32_t nothing_recv_counter = 0;
static uint32_t err_recv_counter = 0;
static TfLunaState_t state;


float tfLunaGetRange() {
    uint16_t distance_sm = ((TfLunaSerialFrame_t*)frame_buf)->distance;
    return distance_sm * 0.01;
}

bool tfLunaParseSerialFrame() {
    for (uint32_t idx = 0; idx < TF_LUNA_SERIAL_FRAME_SIZE; idx++) {
        if (tfLunaNextByte(frame_buf[idx])) {
            return true;
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
            return (byte != HEADER_BYTE) ? true : false;
        default:
            break;
    }
    return false;
}
