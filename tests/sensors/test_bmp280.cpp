/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "barometer/bmp280.h"


TEST(BMP280, test_bmp280ParseData) {
    bmp280Init();
    bmp280Calibrate();

    bmp280CollectData();
    bmp280ParseData();

    float pressure = bmp280GetStaticPressure();
    float temperature = bmp280GetStaticTemperature();
    std::cout << "pressure: " << pressure << std::endl;
    std::cout << "temperature: " << temperature << std::endl;

}

TEST(BMP280, test_bmp280IsInitialized) {
    ASSERT_FALSE(bmp280IsInitialized());
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
