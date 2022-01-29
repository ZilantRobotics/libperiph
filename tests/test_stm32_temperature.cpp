/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file test_stm32_temperature.cpp
 * @author d.ponomarev
 */

#include <iostream>
#include <gtest/gtest.h>
#include "temperature_sensor.h"

TEST(TemperatureSensor, TemperatureSensor_25_celsius) {
    ASSERT_EQ(temperature_sensor_parse(1750), 273+25);
}

TEST(TemperatureSensor, TemperatureSensor_125_celsius) {
    ASSERT_EQ(temperature_sensor_parse(1250), 273+125);
}

TEST(TemperatureSensor, TemperatureSensor_minus_40_celsius) {
    ASSERT_EQ(temperature_sensor_parse(2075), 273-40);
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
