/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_ESC_THUNDER_H_
#define SENSORS_ESC_THUNDER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "ring_buffer.h"

#define ESC_THUNDER_MAX_MSG_LEN     17

typedef struct {
    uint8_t _ring_buffer_storage[ESC_THUNDER_MAX_MSG_LEN];
    RingBuffer _ring_buffer;

    uint16_t spd;
    uint32_t tmos;
    uint32_t tmot;
    float curi;
    float volt;
} EscThunderFeedback;

typedef enum {
    ESC_THUNDER_SPD,
    ESC_THUNDER_TMOS,
    ESC_THUNDER_TMOT,
    ESC_THUNDER_CURI,
    ESC_THUNDER_VOLT,
    ESC_THUNDER_UNKNOWN,
} EscThunderMessage;

/**
  * @brief Initialize the ring buffer
  */
void thunderInit(EscThunderFeedback* esc_thunder);


/**
  * @brief Put the new byte into the ring buffer.
  * If a space char appeared (any message ends up with space symbol), linearize the ring buffer.
  * If the linearized buffer has a message, parse it and return the message type.
  */
EscThunderMessage thunderNextByte(EscThunderFeedback* esc_thunder, uint8_t byte);

/**
  * @brief Parse byte by byte in the DMA buffer
  */
bool thunderParseDma(size_t recv_idx, DmaUartHandler_t* handler, EscThunderFeedback* feedback);


/**
  * @brief The functions below are dedicated for tests only.
  * It is not recommended to use them in an application.
  */
uint8_t hexByteToUint8(uint8_t hex_byte);
uint32_t hexArrayToUint32(const uint8_t* hex_array, uint8_t array_size);
float raw_data_to_voltage(uint16_t raw_data);
float raw_data_to_current(uint16_t raw_data);

bool escThunderIsItPackageStart(const uint8_t* buffer);
void escThunderFlameParse(const uint8_t* buffer, EscThunderFeedback* esc_status);
EscThunderMessage thunderParseMessageInRingBuffer(EscThunderFeedback* esc_thunder);


#ifdef __cplusplus
}
#endif

#endif  // SENSORS_ESC_THUNDER_H_
