/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef LIBPERIPH_COMMON_RING_BUFFER_H_
#define LIBPERIPH_COMMON_RING_BUFFER_H_

#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t* buffer;
    size_t head;
    size_t tail;
    size_t max_size;
    size_t size;
} RingBuffer;

void ringBufferInit(RingBuffer* rb, uint8_t* buffer, size_t size);
void ringBufferPush(RingBuffer* rb, uint8_t data);
size_t ringBufferLinearize(const RingBuffer* rb, uint8_t* linear_buf);

#ifdef __cplusplus
}
#endif

#endif  // LIBPERIPH_COMMON_RING_BUFFER_H_
