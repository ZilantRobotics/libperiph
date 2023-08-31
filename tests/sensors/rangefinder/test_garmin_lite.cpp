/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>
#include <iostream>
#include "rangefinder/garmin_lite/garmin_lite.h"
#include "hal_i2c.h"
#include "ubuntu/sitl_i2c.hpp"

class GarminLite : public SitlI2CSensor {
public:
    GarminLite() : SitlI2CSensor(0xC4) {}
    int8_t callback_on_i2c_transmit(uint8_t id, const uint8_t tx[], uint8_t len) override;
    int8_t callback_on_i2c_receive(uint8_t id, uint8_t* rx, uint8_t len) override;
    void set_distance(float distance);
private:
    uint8_t distance_register[2] = {0, 0};
};

int8_t GarminLite::callback_on_i2c_transmit(uint8_t id, const uint8_t tx[], uint8_t len) {
    return 0;
}

int8_t GarminLite::callback_on_i2c_receive(uint8_t id, uint8_t* rx, uint8_t len) {
    if (len == 2) {
        rx[0] = distance_register[0];
        rx[1] = distance_register[1];
    }

    return 0;
}

void GarminLite::set_distance(float distance) {
    uint16_t distance_sm = distance * 100;
    distance_register[0] = distance_sm >> 8;
    distance_register[1] = distance_sm % 256;
}


static GarminLite garmin_lite;


TEST(garmin_lite, test_garminLiteInit) {
    ASSERT_EQ(LIBPERIPH_OK, garminLiteInit());
}

TEST(garmin_lite, test_garminLiteCollectData) {
    std::array<uint16_t, 6> distances_cm = {0, 100, 255, 256, 1025, 65535};

    for (auto distance_cm : distances_cm) {
        float distance = distance_cm * 0.01f;
        garmin_lite.set_distance(distance);
        ASSERT_EQ(LIBPERIPH_OK, garminLiteCollectData());
        EXPECT_NEAR(distance, garminLiteParseCollectedData(), 0.015);
    }
}

TEST(garmin_lite, test_garminGetSerialNumber) {
    GarminLiteSerialNumber serial_number;
    garminGetSerialNumber(serial_number);
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
