/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "ring_buffer.h"

static const size_t RB_MAX_SIZE = 5;


TEST(RingBufferTets, test_empty) {
    uint8_t ring_buffer_storage[RB_MAX_SIZE] = {};
    uint8_t ring_buffer_linearized[RB_MAX_SIZE] = {};
    RingBuffer ring_buffer;

    ringBufferInit(&ring_buffer, ring_buffer_storage, RB_MAX_SIZE);

    ASSERT_EQ(0, ringBufferLinearize(&ring_buffer, ring_buffer_linearized));
    for (size_t idx = 0; idx < RB_MAX_SIZE; idx++) {
        ASSERT_EQ(ring_buffer_linearized[idx], 0);
    }
}

TEST(RingBufferTets, test_3_elements) {
    uint8_t ring_buffer_storage[RB_MAX_SIZE] = {};
    uint8_t ring_buffer_linearized[RB_MAX_SIZE] = {};
    RingBuffer ring_buffer;

    ringBufferInit(&ring_buffer, ring_buffer_storage, RB_MAX_SIZE);
    for (size_t value = 0; value < 3; value++) {
        ringBufferPush(&ring_buffer, value);
    }

    ASSERT_EQ(3, ringBufferLinearize(&ring_buffer, ring_buffer_linearized));
    ASSERT_EQ(ring_buffer_linearized[0], 0);
    ASSERT_EQ(ring_buffer_linearized[1], 1);
    ASSERT_EQ(ring_buffer_linearized[2], 2);
    ASSERT_EQ(ring_buffer_linearized[3], 0);
    ASSERT_EQ(ring_buffer_linearized[4], 0);
}

TEST(RingBufferTets, test_5_elements) {
    uint8_t ring_buffer_storage[RB_MAX_SIZE] = {};
    uint8_t ring_buffer_linearized[RB_MAX_SIZE] = {};
    RingBuffer ring_buffer;

    ringBufferInit(&ring_buffer, ring_buffer_storage, RB_MAX_SIZE);
    for (size_t value = 0; value < 5; value++) {
        ringBufferPush(&ring_buffer, value);
    }

    ASSERT_EQ(5, ringBufferLinearize(&ring_buffer, ring_buffer_linearized));
    for (size_t idx = 0; idx < RB_MAX_SIZE; idx++) {
        ASSERT_EQ(ring_buffer_linearized[idx], idx);
    }
}

TEST(RingBufferTets, test_7_elements) {
    uint8_t ring_buffer_storage[RB_MAX_SIZE] = {};
    uint8_t ring_buffer_linearized[RB_MAX_SIZE] = {};
    RingBuffer ring_buffer;

    ringBufferInit(&ring_buffer, ring_buffer_storage, RB_MAX_SIZE);
    for (size_t value = 0; value < 7; value++) {
        ringBufferPush(&ring_buffer, value);
    }

    ASSERT_EQ(5, ringBufferLinearize(&ring_buffer, ring_buffer_linearized));
    for (size_t idx = 0; idx < RB_MAX_SIZE; idx++) {
        ASSERT_EQ(ring_buffer_linearized[idx], idx + 2);
    }
}

TEST(RingBufferTets, test_100_elements) {
    uint8_t ring_buffer_storage[RB_MAX_SIZE] = {};
    uint8_t ring_buffer_linearized[RB_MAX_SIZE] = {};
    RingBuffer ring_buffer;

    ringBufferInit(&ring_buffer, ring_buffer_storage, RB_MAX_SIZE);
    for (size_t value = 0; value < 100; value++) {
        ringBufferPush(&ring_buffer, value);
    }

    ASSERT_EQ(5, ringBufferLinearize(&ring_buffer, ring_buffer_linearized));
    for (size_t idx = 0; idx < RB_MAX_SIZE; idx++) {
        ASSERT_EQ(ring_buffer_linearized[idx], idx + 95);
    }
}

TEST(RingBufferTets, test_500_elements) {
    uint8_t ring_buffer_storage[RB_MAX_SIZE] = {};
    uint8_t ring_buffer_linearized[RB_MAX_SIZE] = {};
    RingBuffer ring_buffer;

    ringBufferInit(&ring_buffer, ring_buffer_storage, RB_MAX_SIZE);
    for (size_t value = 0; value < 500; value++) {
        ringBufferPush(&ring_buffer, value);
    }

    ringBufferLinearize(&ring_buffer, ring_buffer_linearized);
    for (size_t idx = 0; idx < RB_MAX_SIZE; idx++) {
        ASSERT_EQ(ring_buffer_linearized[idx], (idx + 495) % 256);
    }
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
