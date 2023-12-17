/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "barometer/bmp280.h"
#include "ubuntu/sitl_i2c.hpp"

struct Bmp280 : public SitlI2CSensor {
    Bmp280() : SitlI2CSensor(0xC4) {}
    int8_t callback_on_i2c_transmit(uint8_t id, const uint8_t tx[], uint8_t len) override {
        return i2c_tx_status;
    }
    int8_t callback_on_i2c_receive(uint8_t id, uint8_t* rx, uint8_t len) override {
        return i2c_rx_status;
    }

    int8_t i2c_tx_status = 0;
    int8_t i2c_rx_status = 0;
};

static Bmp280 bmp280;

TEST(BMP280, test_bmp280ParseData_normal) {
    bmp280.i2c_tx_status = 0;
    bmp280.i2c_rx_status = 0;

    bmp280Init();
    bmp280Calibrate();

    ASSERT_TRUE(bmp280CollectData() >= 0);
    bmp280ParseData();

    bmp280GetStaticPressure();
    bmp280GetStaticTemperature();
}

TEST(BMP280, test_bmp280ParseData_no_rx_response) {
    bmp280Init();
    bmp280Calibrate();

    bmp280.i2c_tx_status = -1;
    bmp280.i2c_rx_status = 0;

    ASSERT_FALSE(bmp280CollectData() >= 0);
}

TEST(BMP280, test_bmp280ParseData_no_tx_response) {
    bmp280Init();
    bmp280Calibrate();

    bmp280.i2c_tx_status = 0;
    bmp280.i2c_rx_status = -1;

    ASSERT_FALSE(bmp280CollectData() >= 0);
}

TEST(BMP280, test_bmp280IsInitialized) {
    ASSERT_FALSE(bmp280IsInitialized());
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
