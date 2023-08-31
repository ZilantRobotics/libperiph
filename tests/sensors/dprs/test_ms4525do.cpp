/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
#include "differential_pressure/ms4525do.h"


#define I2C_RESPONSE_SIZE   4

TEST(ms4525do, test_ms4525doInit) {
    ASSERT_EQ(LIBPERIPH_OK, ms4525doInit());
}

TEST(ms4525do, test_ms4525CollectData) {
    ms4525CollectData();
}

/**
 * @brief MS4525DO. Page 5. Sensor Output at Significant Percentages
 */
void fill_pressure(uint16_t table_value) {
    uint8_t ms4525do_rx_buf[I2C_RESPONSE_SIZE];
    uint16_t raw = table_value;
    ms4525do_rx_buf[0] = (0x3F & (raw >> 8));
    ms4525do_rx_buf[1] = 0xFF & raw;
    ms4525doFillBuffer(ms4525do_rx_buf);
}
TEST(ms4525do, pressure_less_then_min) {
    std::vector<std::pair<uint16_t, float>> pressure_table {{
        {0x0000, 8618.45},
        {0x0666, 6894.757f},
        {0x2000, 0},
        {0x399A, -6894.757f},
        {0x3FFF, -8618.45},
    }};

    for (auto pressure_pair : pressure_table) {
        fill_pressure(pressure_pair.first);
        DifferentialPressureData data = ms4525ParseCollectedData();
        ASSERT_TRUE(abs(data.diff_pressure - pressure_pair.second) < 2.0);
    }
}


/**
 * @brief MS4525DO. Page 5. Temperature Output vs Counts
 */
void fill_temperature(uint16_t table_value) {
    uint8_t ms4525do_rx_buf[I2C_RESPONSE_SIZE];
    uint16_t raw = table_value;
    ms4525do_rx_buf[2] = (0xFF & (raw >> 3));
    ms4525do_rx_buf[3] = (0x07 & raw) << 5;
    ms4525doFillBuffer(ms4525do_rx_buf);
}
TEST(ms4525do, test_ms4525ParseCollectedData) {
    std::vector<std::pair<uint16_t, float>> temperature_table {{
        {0x0000, -50},
        {0x01FF, 0},
        {0x0266, 10},
        {0x0565, 85},
        {0x07FF, 150},
    }};

    for (auto temperature_pair : temperature_table) {
        fill_temperature(temperature_pair.first);
        DifferentialPressureData data = ms4525ParseCollectedData();
        ASSERT_EQ(round(data.temperature), temperature_pair.second);
    }
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
