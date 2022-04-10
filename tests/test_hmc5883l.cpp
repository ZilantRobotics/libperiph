/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file test_hmc5883l.cpp
 * @author d.ponomarev
 */

#include <iostream>
#include <gtest/gtest.h>
#include "mag_hmc5883l.h"

TEST(hmc5883l, hmc5883lParseNormal) {
    uint8_t raw[6] = {
        4, 66,
        4, 66,
        4, 66
    };
    float x, y, z;

    hmc5883lParse((uint8_t*)raw);
    hmc5883GetMeasurement(&x, &y, &z);
    ASSERT_EQ(x, 1.0);
    ASSERT_EQ(y, 1.0);
    ASSERT_EQ(z, 1.0);
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
