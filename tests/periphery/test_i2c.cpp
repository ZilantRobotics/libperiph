/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "hal_i2c.h"
#include "libperiph_common.h"

uint8_t ubuntu_i2c_id = 42;
uint8_t ubuntu_i2c_buffer[256] = {};

TEST(hal_i2c, test_i2cTransmit) {
    uint8_t tx_buffer = 42;
    ASSERT_EQ(LIBPERIPH_OK, i2cTransmit(0, &tx_buffer, 1));
    ASSERT_EQ(LIBPERIPH_ERROR, i2cTransmit(0, NULL, 0));
}

TEST(hal_i2c, test_i2cReceive) {
    uint8_t rx_buffer[2] = {};
    ASSERT_EQ(LIBPERIPH_OK, i2cReceive(0, rx_buffer, 2));
    ASSERT_EQ(LIBPERIPH_ERROR, i2cReceive(0, NULL, 2));
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
