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
#include "ubuntu/sitl_i2c.hpp"

uint8_t ubuntu_i2c_id = 42;
uint8_t ubuntu_i2c_buffer[256] = {};

class SimpleSitlI2CSensor : public SitlI2CSensor {
public:
    SimpleSitlI2CSensor() : SitlI2CSensor(0xFF) {}
    int8_t callback_on_i2c_transmit(uint8_t id, const uint8_t tx[], uint8_t len) override {
        return 0;
    }
    int8_t callback_on_i2c_receive(uint8_t id, uint8_t* rx, uint8_t len) override {
        return 0;
    }
};

static SimpleSitlI2CSensor simple_sitl_i2c_sensor;

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
