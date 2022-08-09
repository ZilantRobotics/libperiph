/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file test_algorithms.cpp
 * @author d.ponomarev
 * @date Apr 16, 2021
 */

#include <iostream>
#include <gtest/gtest.h>
#include "esc/flame.h"

TEST(EscFlame, EscFlameCheckWrongPackageFirst) {
    uint8_t package_buffer[24] = {154,  22,     4,      2,      98,     147,
                                  0,    0,      0,      47,     0,      0,
                                  7,    32,     0,      0,      0,      0,
                                  12,   176,    12,     222,    168,    3
                                 };

    bool is_it_package = EscFlameIsItPackageStart(package_buffer);
    ASSERT_TRUE(!is_it_package);
}

TEST(EscFlame, EscFlameParseVoltageRpmPowerFirstWithoutPwm) {
    EscStatus_t esc_status = {0};
    uint8_t package_buffer[24] = {155,  22,     4,      2,      98,     147,
                                  0,    0,      0,      47,     0,      0,
                                  7,    32,     0,      0,      0,      0,
                                  12,   176,    12,     222,    168,    3
                                 };

    bool is_it_package = EscFlameIsItPackageStart(package_buffer);
    EscFlameParse(package_buffer, &esc_status);

    ASSERT_TRUE(is_it_package);
    ASSERT_TRUE(esc_status.voltage > 25 && esc_status.voltage < 35);
    ASSERT_TRUE(esc_status.rpm == 0);
    ASSERT_TRUE(esc_status.power_rating_pct == 0);
}

TEST(EscFlame, EscFlameParseVoltageRpmPowerSecondWithPwmTwentyPercent) {
    EscStatus_t esc_status = {0};
    uint8_t package_buffer[24] = {155,  22,     4,      2,      2,      216,
                                  0,    211,    0,      211,    0,      57,
                                  7,    33,     1,      88,     0,      0,
                                  12,   188,    12,     248,    189,    5
                                 };

    bool is_it_package = EscFlameIsItPackageStart(package_buffer);
    EscFlameParse(package_buffer, &esc_status);

    ASSERT_TRUE(is_it_package);
    ASSERT_TRUE(esc_status.voltage > 25 && esc_status.voltage < 35);
    ASSERT_TRUE(esc_status.rpm == 1045);
    ASSERT_TRUE(esc_status.power_rating_pct == 20);
}

TEST(EscFlame, EscFlameParseVoltageRpmPowerThirdWithPwmTwentyPercent) {
    EscStatus_t esc_status = {0};
    uint8_t package_buffer[24] = {155,  22,     4,      2,      108,    242,
                                  0,    81,     0,      81,     0,      21,
                                  7,    31,     1,      86,     0,      0,
                                  12,   170,    12,     226,    237,    4
                                 };

    bool is_it_package = EscFlameIsItPackageStart(package_buffer);
    EscFlameParse(package_buffer, &esc_status);

    ASSERT_TRUE(is_it_package);
    ASSERT_TRUE(esc_status.voltage > 25 && esc_status.voltage < 35);
    ASSERT_TRUE(esc_status.rpm == 385);
    ASSERT_TRUE(esc_status.power_rating_pct == 7);
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
