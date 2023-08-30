/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "current_sensor/acs712.h"

TEST(acs712, test_acs712ConvertToCurrent) {
    acs712SetCoefficient(0.01);
    acs712SetCalibratedValue(1000);
    ASSERT_EQ(acs712ConvertToCurrent(1000), 0);
    ASSERT_EQ(acs712ConvertToCurrent(1500), 5);
    ASSERT_EQ(acs712ConvertToCurrent(500), -5);
}

TEST(acs712, test_acs712PerformIterationOfCalibration) {
    acs712SetCoefficient(0.01);
    acs712SetCalibratedValue(1000);

    for (uint_fast16_t idx = 0; idx < 3000; idx++) {
        ASSERT_FALSE(acs712PerformIterationOfCalibration(1500));
    }
    ASSERT_TRUE(acs712PerformIterationOfCalibration(1500));

    ASSERT_EQ(acs712ConvertToCurrent(1000), -5);
    ASSERT_EQ(acs712ConvertToCurrent(1500), 0);
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
