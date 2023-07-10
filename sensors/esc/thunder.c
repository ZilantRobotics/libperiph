/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "thunder.h"
#include <string.h>


typedef struct {
    const uint8_t* header;
    uint8_t msg_len;
    uint8_t head_len;
    uint8_t val_len;
    void (*parse)(EscThunderFeedback*, const uint8_t*);
} EscThunderMessageMetadata;


static void parseSpd(EscThunderFeedback* esc_thunder, const uint8_t* value);
static void parseTmos(EscThunderFeedback* esc_thunder, const uint8_t* value);
static void parseTmot(EscThunderFeedback* esc_thunder, const uint8_t* value);
static void parseCuri(EscThunderFeedback* esc_thunder, const uint8_t* value);
static void parseVolt(EscThunderFeedback* esc_thunder, const uint8_t* value);
static bool thunderCheckHeader(const uint8_t* buffer, EscThunderMessage msg_idx);
static bool hexArrayIsCorrect(const uint8_t* hex_array, uint8_t array_size);


const static EscThunderMessageMetadata messages[5] = {  // NOLINT
    {.header = (uint8_t*)"SPD:",  .msg_len = 9,  .head_len = 4, .val_len = 4, .parse = &parseSpd},
    {.header = (uint8_t*)"TMOS:", .msg_len = 10, .head_len = 5, .val_len = 4, .parse = &parseTmos},
    {.header = (uint8_t*)"TMOT:", .msg_len = 10, .head_len = 5, .val_len = 4, .parse = &parseTmot},
    {.header = (uint8_t*)"CURI:", .msg_len = 10, .head_len = 5, .val_len = 4, .parse = &parseCuri},
    {.header = (uint8_t*)"VOLT:", .msg_len = 10, .head_len = 5, .val_len = 4, .parse = &parseVolt}
};


void thunderInit(EscThunderFeedback* esc_thunder) {
    if (esc_thunder == NULL) {
        return;
    }

    ringBufferInit(&esc_thunder->_ring_buffer,
                   esc_thunder->_ring_buffer_storage,
                   ESC_THUNDER_MAX_MSG_LEN);
    for (size_t idx = 0; idx < ESC_THUNDER_MAX_MSG_LEN; idx++) {
        thunderNextByte(esc_thunder, '.');
    }

    esc_thunder->curi = 0;
    esc_thunder->spd = 0;
    esc_thunder->tmos = 0;
    esc_thunder->tmot = 0;
    esc_thunder->volt = 0;
}

EscThunderMessage thunderNextByte(EscThunderFeedback* esc_thunder, uint8_t byte) {
    if (esc_thunder == NULL || esc_thunder->_ring_buffer.buffer == NULL) {
        return ESC_THUNDER_UNKNOWN;
    }

    ringBufferPush(&esc_thunder->_ring_buffer, byte);
    if (byte == ' ') {
        return thunderParseMessageInRingBuffer(esc_thunder);
    }

    return ESC_THUNDER_UNKNOWN;
}

bool thunderParseDma(size_t recv_idx, DmaUartHandler_t* handler, EscThunderFeedback* feedback) {
    if (handler == NULL || recv_idx >= handler->size || feedback == NULL) {
        return false;
    }

    bool res = false;

    uint8_t recv_bytes;
    if (recv_idx > handler->saved_idx) {
        recv_bytes = recv_idx - handler->saved_idx;
        for (uint8_t idx = handler->saved_idx + 1; idx < recv_idx + 1; idx++) {
            res |= (ESC_THUNDER_UNKNOWN != thunderNextByte(feedback, handler->buf[idx]));
        }
    } else if (recv_idx < handler->saved_idx) {
        for (uint8_t idx = handler->saved_idx + 1; idx < handler->size; idx++) {
            res |= (ESC_THUNDER_UNKNOWN != thunderNextByte(feedback, handler->buf[idx]));
        }
        for (uint8_t idx = 0; idx <= recv_idx; idx++) {
            res |= (ESC_THUNDER_UNKNOWN != thunderNextByte(feedback, handler->buf[idx]));
        }
    }

    handler->saved_idx = recv_idx;

    return res;
}

EscThunderMessage thunderParseMessageInRingBuffer(EscThunderFeedback* esc_thunder) {
    uint8_t lin_buf[ESC_THUNDER_MAX_MSG_LEN];
    ringBufferLinearize(&esc_thunder->_ring_buffer, lin_buf);

    for (uint_fast8_t msg_idx = 0; msg_idx < ESC_THUNDER_UNKNOWN; msg_idx++) {
        EscThunderMessage msg_type = (EscThunderMessage)msg_idx;
        const uint8_t* header = &lin_buf[ESC_THUNDER_MAX_MSG_LEN - messages[msg_idx].msg_len];
        const uint8_t* value = &lin_buf[ESC_THUNDER_MAX_MSG_LEN - 5];
        if (thunderCheckHeader(header, msg_type) && hexArrayIsCorrect(value, 4)) {
            if (messages[msg_idx].parse != NULL) {
                messages[msg_idx].parse(esc_thunder, value);
            }
            return msg_type;
        }
    }

    return ESC_THUNDER_UNKNOWN;
}

bool thunderCheckHeader(const uint8_t* buffer, EscThunderMessage msg_idx) {
    for (uint_fast8_t byte_idx = 0; byte_idx < messages[msg_idx].head_len; byte_idx++) {
        if (buffer[byte_idx] != messages[msg_idx].header[byte_idx]) {
            return false;
        }
    }
    return true;
}

void parseSpd(EscThunderFeedback* esc_thunder, const uint8_t* value) {
    esc_thunder->spd = hexArrayToUint32(value, 4);
}
void parseTmos(EscThunderFeedback* esc_thunder, const uint8_t* value) {
    esc_thunder->tmos = hexArrayToUint32(value, 4);
}
void parseTmot(EscThunderFeedback* esc_thunder, const uint8_t* value) {
    esc_thunder->tmot = hexArrayToUint32(value, 4);
}
void parseCuri(EscThunderFeedback* esc_thunder, const uint8_t* value) {
    esc_thunder->curi = 0.01 * hexArrayToUint32(value, 4);
}
void parseVolt(EscThunderFeedback* esc_thunder, const uint8_t* value) {
    esc_thunder->volt = 0.01 * hexArrayToUint32(value, 4);
}

bool hexArrayIsCorrect(const uint8_t* hex_array, uint8_t array_size) {
    for (uint_fast8_t idx = 0; idx < array_size; idx++) {
        uint8_t hex = hex_array[idx];
        if (!(hex >= '0' && hex <= '9') && !(hex >= 'A' && hex <= 'F')) {
            return false;
        }
    }
    return true;
}

uint32_t hexArrayToUint32(const uint8_t* hex_array, uint8_t array_size) {
    uint32_t res = 0;
    for (int_fast8_t idx = array_size - 1; idx >= 0; idx--) {
        uint8_t shift = (array_size - idx - 1) * 4;
        res += hexByteToUint8(hex_array[idx]) << shift;
    }
    return res;
}

uint8_t hexByteToUint8(uint8_t hex_byte) {
    return (hex_byte <= '9') ? hex_byte - '0' : 10 + hex_byte - 'A';
}
