/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file test_bmp280.cpp
 * @author d.ponomarev
 */

#include <iostream>
#include <gtest/gtest.h>
#include "barometer/bmp280.h"


// TEST(BMP280, bmp280ParseData) {

// }

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
