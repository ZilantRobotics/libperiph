/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
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

#define FIRST_BYTE      155
#define SECOND_BYTE     22

// Real data set
const uint8_t PWM_0_PERCENT[24] = {
    155,    22,     4,      2,      98,
    147,    0,      0,      0,      47,
    0,      0,      7,      32,     0,
    0,      0,      0,      12,     176,
    12,     222,    168,    3
};

const uint8_t PWM_20_PERCENT[24] = {
    155,    22,     4,      2,      2,
    216,    0,      211,    0,      211,
    0,      57,     7,      33,     1,
    88,     0,      0,      12,     188,
    12,     248,    189,    5
};

const uint8_t PWM_7_PERCENT[24] = {
    155,    22,     4,      2,      108,
    242,    0,      81,     0,      81,
    0,      21,     7,      31,     1,
    86,     0,      0,      12,     170,
    12,     226,    237,    4
};

void check_pwm_0_percent(const EscFlameStatus_t& esc_status) {
    ASSERT_TRUE(esc_status.voltage > 25 && esc_status.voltage < 35);
    ASSERT_TRUE(esc_status.rpm == 0);
    ASSERT_TRUE(esc_status.power_rating_pct == 0);
}

void check_pwm_7_percent(const EscFlameStatus_t& esc_status) {
    ASSERT_TRUE(esc_status.voltage > 25 && esc_status.voltage < 35);
    ASSERT_TRUE(esc_status.rpm == 385);
    ASSERT_TRUE(esc_status.power_rating_pct == 7);
}

void check_pwm_20_percent(const EscFlameStatus_t& esc_status) {
    ASSERT_TRUE(esc_status.voltage > 25 && esc_status.voltage < 35);
    ASSERT_TRUE(esc_status.rpm == 1045);
    ASSERT_TRUE(esc_status.power_rating_pct == 20);
}

void fill_buffer_with_frame(UartDmaParser_t& parser, size_t parser_buf_idx, const uint8_t* real_case) {
    if (parser_buf_idx + ESC_FLAME_PACKAGE_SIZE <= UART_BUFFER_SIZE) {
        memcpy(parser.buf + parser_buf_idx, real_case, ESC_FLAME_PACKAGE_SIZE);
    } else {
        size_t first_part_size = UART_BUFFER_SIZE - parser_buf_idx;
        size_t second_part_size = ESC_FLAME_PACKAGE_SIZE - first_part_size;
        memcpy(parser.buf + parser_buf_idx, real_case, first_part_size);
        memcpy(parser.buf, real_case + first_part_size, second_part_size);
    }
}

TEST(EscFlame, escFlameIsItPackageStart) {
    // wrong argument
    ASSERT_TRUE(!escFlameIsItPackageStart(NULL));

    // wrong first byte
    uint8_t package_wrong_first_byte[] = {FIRST_BYTE, 0};
    ASSERT_TRUE(!escFlameIsItPackageStart(package_wrong_first_byte));

    // wrong second byte
    uint8_t package_wrong_second_byte[] = {0, SECOND_BYTE};
    ASSERT_TRUE(!escFlameIsItPackageStart(package_wrong_second_byte));

    // package start
    uint8_t package_start[] = {FIRST_BYTE, SECOND_BYTE};
    ASSERT_TRUE(escFlameIsItPackageStart(package_start));
}

TEST(EscFlame, escFlameParse) {
    EscFlameStatus_t esc_status{};

    // Wrong: first argument
    escFlameParse(NULL, &esc_status);
    ASSERT_TRUE(esc_status.rpm == 0);

    // Wrong: second argument
    const uint8_t raw_package_buffer[24] = {};
    escFlameParse(raw_package_buffer, NULL);

    // Correct: 0%
    ASSERT_TRUE(escFlameIsItPackageStart(PWM_0_PERCENT));
    escFlameParse(PWM_0_PERCENT, &esc_status);
    check_pwm_0_percent(esc_status);

    // Correct: 20%
    ASSERT_TRUE(escFlameIsItPackageStart(PWM_20_PERCENT));
    escFlameParse(PWM_20_PERCENT, &esc_status);
    check_pwm_20_percent(esc_status);

    // Correct: 7%
    ASSERT_TRUE(escFlameIsItPackageStart(PWM_7_PERCENT));
    escFlameParse(PWM_7_PERCENT, &esc_status);
    check_pwm_7_percent(esc_status);
}

TEST(EscFlame, escFlameParseDma_wrong_args) {
    UartDmaParser_t parser;
    EscFlameStatus_t esc_status;
    size_t recv_idx = 0;

    ASSERT_FALSE(escFlameParseDma(UART_BUFFER_SIZE, &parser, &esc_status));
    ASSERT_FALSE(escFlameParseDma(recv_idx, NULL, &esc_status));
    ASSERT_FALSE(escFlameParseDma(recv_idx, &parser, NULL));
}

TEST(EscFlame, escFlameParseDma_empty) {
    UartDmaParser_t parser;
    EscFlameStatus_t esc_status;
    size_t recv_idx = 0;

    escFlameParseDma(recv_idx, &parser, &esc_status);
}

TEST(EscFlame, escFlameParseDma_perfect_sequence) {
    UartDmaParser_t parser;
    EscFlameStatus_t esc_status;

    for (size_t package_idx = 0; package_idx < 9; package_idx++) {
        size_t parser_buf_idx = (package_idx * ESC_FLAME_PACKAGE_SIZE) % UART_BUFFER_SIZE;
        size_t recv_idx = (parser_buf_idx + ESC_FLAME_PACKAGE_SIZE - 1) % UART_BUFFER_SIZE;

        const uint8_t* real_case;
        if (package_idx % 3 == 0) {
            real_case = PWM_0_PERCENT;
        } else if (package_idx % 3 == 1) {
            real_case = PWM_7_PERCENT;
        } else if (package_idx % 3 == 2) {
            real_case = PWM_20_PERCENT;
        }

        fill_buffer_with_frame(parser, parser_buf_idx, real_case);

        ASSERT_TRUE(escFlameParseDma(recv_idx, &parser, &esc_status));
        ASSERT_EQ(parser.saved_idx, recv_idx);

        if (package_idx % 3 == 0) {
            check_pwm_0_percent(esc_status);
        } else if (package_idx % 3 == 1) {
            check_pwm_7_percent(esc_status);
        } else if (package_idx % 3 == 2) {
            check_pwm_20_percent(esc_status);
        }
    }
}

TEST(EscFlame, escFlameParseDma_offset_sequence) {
    UartDmaParser_t parser;
    EscFlameStatus_t esc_status;

    for (size_t package_idx = 0; package_idx < 4; package_idx++) {
        size_t parser_buf_idx = (12 + package_idx * ESC_FLAME_PACKAGE_SIZE) % UART_BUFFER_SIZE;
        size_t recv_idx = (parser_buf_idx + ESC_FLAME_PACKAGE_SIZE - 1) % UART_BUFFER_SIZE;

        const uint8_t* real_case;
        if (package_idx % 3 == 0) {
            real_case = PWM_0_PERCENT;
        } else if (package_idx % 3 == 1) {
            real_case = PWM_7_PERCENT;
        } else if (package_idx % 3 == 2) {
            real_case = PWM_20_PERCENT;
        }

        fill_buffer_with_frame(parser, parser_buf_idx, real_case);

        ASSERT_TRUE(escFlameParseDma(recv_idx, &parser, &esc_status));
        ASSERT_EQ(parser.saved_idx, recv_idx);

        if (package_idx % 3 == 0) {
            check_pwm_0_percent(esc_status);
        } else if (package_idx % 3 == 1) {
            check_pwm_7_percent(esc_status);
        } else if (package_idx % 3 == 2) {
            check_pwm_20_percent(esc_status);
        }

        size_t intermediate_idx;
        intermediate_idx = (recv_idx + 0) % UART_BUFFER_SIZE;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
        intermediate_idx = (recv_idx + 1) % UART_BUFFER_SIZE;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
        intermediate_idx = (recv_idx + 2) % UART_BUFFER_SIZE;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
    }
}

TEST(EscFlame, escFlameParseDma_poll_faster_than_measure) {
    UartDmaParser_t parser;
    EscFlameStatus_t esc_status;

    for (size_t package_idx = 0; package_idx < 9; package_idx++) {
        size_t parser_buf_idx = (package_idx * ESC_FLAME_PACKAGE_SIZE) % UART_BUFFER_SIZE;
        size_t recv_idx = (parser_buf_idx + ESC_FLAME_PACKAGE_SIZE - 1) % UART_BUFFER_SIZE;

        const uint8_t* real_case;
        if (package_idx % 3 == 0) {
            real_case = PWM_0_PERCENT;
        } else if (package_idx % 3 == 1) {
            real_case = PWM_7_PERCENT;
        } else if (package_idx % 3 == 2) {
            real_case = PWM_20_PERCENT;
        }

        fill_buffer_with_frame(parser, parser_buf_idx, real_case);

        ASSERT_TRUE(escFlameParseDma(recv_idx, &parser, &esc_status));
        ASSERT_EQ(parser.saved_idx, recv_idx);

        if (package_idx % 3 == 0) {
            check_pwm_0_percent(esc_status);
        } else if (package_idx % 3 == 1) {
            check_pwm_7_percent(esc_status);
        } else if (package_idx % 3 == 2) {
            check_pwm_20_percent(esc_status);
        }

        size_t intermediate_idx;
        intermediate_idx = (recv_idx + 0) % UART_BUFFER_SIZE;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
        intermediate_idx = (recv_idx + 1) % UART_BUFFER_SIZE;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
        intermediate_idx = (recv_idx + 2) % UART_BUFFER_SIZE;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
    }
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
