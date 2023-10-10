/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "esc/flame.h"

#define FIRST_BYTE      155
#define SECOND_BYTE     22

/**
  * @brief Real packages
  */
struct EscFlamePackage {
    const uint8_t raw[24];
    const EscFlame_t reference;
};

const EscFlamePackage pwm_0{
    {
        155, 22, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 217, 0, 0, 0, 0, 211, 211, 8, 0, 59, 3
    },
    {
        .voltage = 21.7,
        .current = 0.0,
        .temperature = 312.0,
        .rpm = 0,
        .power_rating_pct = 0,
    }
};

void ASSERT_FB_EQUAL(const EscFlame_t& first, const EscFlame_t& second) {
    ASSERT_NEAR(first.voltage, second.voltage, 2.0);
    ASSERT_NEAR(first.current, second.current, 0.1);
    ASSERT_NEAR(first.temperature, second.temperature, 2.0);
    ASSERT_EQ(first.rpm, second.rpm);
    ASSERT_EQ(first.power_rating_pct, second.power_rating_pct);
}

void fill_buffer_with_frame(DmaUartHandler_t& parser, size_t parser_buf_idx, const uint8_t* real_case) {
    if (parser_buf_idx + ESC_FLAME_PACKAGE_SIZE <= parser.size) {
        memcpy(parser.buf + parser_buf_idx, real_case, ESC_FLAME_PACKAGE_SIZE);
    } else {
        size_t first_part_size = parser.size - parser_buf_idx;
        size_t second_part_size = ESC_FLAME_PACKAGE_SIZE - first_part_size;
        memcpy(parser.buf + parser_buf_idx, real_case, first_part_size);
        memcpy(parser.buf, real_case + first_part_size, second_part_size);
    }
}


TEST(EscFlame, escFlameParse) {
    EscFlame_t esc_status{};

    // Wrong: first argument
    escFlameParse(NULL, &esc_status);
    ASSERT_TRUE(esc_status.rpm == 0);

    // Wrong: second argument
    const uint8_t raw_package_buffer[24] = {};
    escFlameParse(raw_package_buffer, NULL);

    // Correct: 0%
    ASSERT_TRUE(escFlameIsItPackageStart(pwm_0.raw));
    escFlameParse(pwm_0.raw, &esc_status);
    ASSERT_FB_EQUAL(esc_status, pwm_0.reference);

}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    escMotorNumberOfPoles(0);
    escMotorNumberOfPoles(28);
    escSetFlameParameters();
    escSetAlphaParameters();
    return RUN_ALL_TESTS();
}
