/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "magnetometer/rm3100.h"

TEST(rm3100, test_rm3100Init) {
    ASSERT_EQ(rm3100Init(), -1);
}

TEST(rm3100, test_rm3100Measure) {
    rm3100Measure();
}

TEST(rm3100, test_rm310GetMeasurement) {
    float x, y, z;
    rm310GetMeasurement(&x, &y, &z);

    rm310GetMeasurement(NULL, &y, &z);
    rm310GetMeasurement(&x, NULL, &z);
    rm310GetMeasurement(&x, &y, NULL);
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
