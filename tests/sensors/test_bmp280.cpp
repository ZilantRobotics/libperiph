/*
 * Copyright (C) 2021-2024 Dmitry Ponomarev <ponomarevda96@gmail.com>
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
        if (len > 0) {
            latest_reg = tx[0];
        }
        return i2c_tx_status;
    }
    int8_t callback_on_i2c_receive(uint8_t id, uint8_t* rx, uint8_t len) override {
        if (latest_reg == 0xF7 && len == 6) {
            // we can fill pressure and temperature here
        }
        return i2c_rx_status;
    }

    uint8_t latest_reg = 0;
    int8_t i2c_tx_status = 0;
    int8_t i2c_rx_status = 0;
};

static Bmp280 bmp280;

TEST(BMP280, test_bmp280IsInitialized) {
    ASSERT_FALSE(bmp280IsInitialized());
}

TEST(BMP280, test_bmp280GetData) {
    bmp280.i2c_tx_status = 0;
    bmp280.i2c_rx_status = 0;

    bmp280Init();

    BarometerMeasurements data;
    ASSERT_EQ(bmp280GetData(&data), -LIBPERIPH_BPM280_VERIFICATION_PRESSURE_OUT_OF_BOUND);
}

TEST(BMP280, test_bmp280ParseCollectedData) {
    bmp280.i2c_tx_status = 0;
    bmp280.i2c_rx_status = 0;
    bmp280Init();
    ASSERT_EQ(bmp280CollectData(), LIBPERIPH_BPM280_OK);
    BarometerMeasurements data;

    // Normal
    ASSERT_EQ(bmp280ParseCollectedData(&data), LIBPERIPH_BPM280_OK);

    // Bad argument
    ASSERT_EQ(bmp280ParseCollectedData(NULL), -LIBPERIPH_BPM280_INTERNAL_ERROR);
}

TEST(BMP280, test_bmp280CollectData_no_rx_response) {
    bmp280Init();

    bmp280.i2c_tx_status = -1;
    bmp280.i2c_rx_status = 0;

    ASSERT_FALSE(bmp280CollectData() >= 0);
}

TEST(BMP280, test_bmp280CollectData_no_tx_response) {
    bmp280Init();

    bmp280.i2c_tx_status = 0;
    bmp280.i2c_rx_status = -1;

    ASSERT_FALSE(bmp280CollectData() >= 0);
}

TEST(BMP280, test_bmp280VerifyData) {
    bmp280Init();  // reset internal state
    BarometerMeasurements data;

    // Normal
    data = BarometerMeasurements{100000, 273};
    ASSERT_EQ(bmp280VerifyData(&data), LIBPERIPH_BPM280_OK);

    // Pressure out of bound
    data = BarometerMeasurements{29999, 273};
    ASSERT_EQ(bmp280VerifyData(&data), -LIBPERIPH_BPM280_VERIFICATION_PRESSURE_OUT_OF_BOUND);
    data = BarometerMeasurements{110001, 273};
    ASSERT_EQ(bmp280VerifyData(&data), -LIBPERIPH_BPM280_VERIFICATION_PRESSURE_OUT_OF_BOUND);

    // Temperature out of bound
    data = BarometerMeasurements{100000, 232};
    ASSERT_EQ(bmp280VerifyData(&data), -LIBPERIPH_BPM280_VERIFICATION_TEMPERATURE_OUT_OF_BOUND);
    data = BarometerMeasurements{100000, 369};
    ASSERT_EQ(bmp280VerifyData(&data), -LIBPERIPH_BPM280_VERIFICATION_TEMPERATURE_OUT_OF_BOUND);
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
