/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ring_buffer.h"

void ringBufferInit(RingBuffer* rb, uint8_t* buffer, size_t size) {
    rb->buffer = buffer;
    rb->head = 0;
    rb->tail = 0;
    rb->max_size = size;
    rb->size = 0;
}

void ringBufferPush(RingBuffer* rb, uint8_t data) {
    rb->buffer[rb->tail] = data;
    rb->tail = (rb->tail + 1) % rb->max_size;
    rb->size++;
    if (rb->size > rb->max_size) {
        rb->head = (rb->head + 1) % rb->max_size;
        rb->size = rb->max_size;
    }
}

size_t ringBufferLinearize(const RingBuffer* rb, uint8_t* linear_buf) {
    size_t lin_buf_idx = 0;
    for (size_t rb_idx = rb->head; rb_idx < rb->head + rb->size; rb_idx++) {
        linear_buf[lin_buf_idx] = rb->buffer[rb_idx % rb->max_size];
        lin_buf_idx++;
    }
    return rb->size;
}
