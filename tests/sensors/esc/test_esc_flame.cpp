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

struct EscFlamePackage {
    const uint8_t raw[24];
    const EscFlame_t reference;
};

const EscFlamePackage pwm_0{
    {
        155, 22, 4, 2, 0, 1, 0, 0, 0, 47, 0, 0, 7, 32, 0, 0, 0, 0, 12, 176, 12, 222, 109, 2
    },
    {
        .voltage = 30,
        .rpm = 0,
        .power_rating_pct = 0
    }
};

const EscFlamePackage pwm_7{
    {
        155, 22, 4, 2, 108, 242, 0, 81, 0, 81, 0, 21, 7, 31, 1, 86, 0, 0, 12, 170, 12, 226, 237, 4
    },
    {
        .voltage = 30,
        .rpm = 385,
        .power_rating_pct = 7
    }
};

const EscFlamePackage pwm_20{
    {
        155, 22, 4, 2, 2, 216, 0, 211, 0, 211, 0, 57, 7, 33, 1, 88, 0, 0, 12, 188, 12, 248, 189, 5
    },
    {
        .voltage = 30,
        .rpm = 1045,
        .power_rating_pct = 20
    }
};

const EscFlamePackage pwm_25{
    {
        155, 22, 4, 2, 3, 27, 0, 149, 0, 149, 0, 23, 4, 136, 1, 84, 0, 0, 13, 44, 13, 69, 130, 3
    },
    {
        .voltage = 20,
        .rpm = 422,
        .power_rating_pct = 14
    }
};

const EscFlamePackage pwm_55{
    {
        155, 22, 4, 2, 6, 155, 1, 250, 1, 250, 0, 81, 4, 52, 1, 93, 0, 0, 13, 0, 13, 31, 110, 4
    },
    {
        .voltage = 20,
        .rpm = 1486,
        .power_rating_pct = 49
    }
};


void ASSERT_FB_EQUAL(const EscFlame_t& first, const EscFlame_t& second) {
    ASSERT_NEAR(first.voltage, second.voltage, 2.0);
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

    // Correct: 7%
    ASSERT_TRUE(escFlameIsItPackageStart(pwm_7.raw));
    escFlameParse(pwm_7.raw, &esc_status);
    ASSERT_FB_EQUAL(esc_status, pwm_7.reference);

    // Correct: 20%
    ASSERT_TRUE(escFlameIsItPackageStart(pwm_20.raw));
    escFlameParse(pwm_20.raw, &esc_status);
    ASSERT_FB_EQUAL(esc_status, pwm_20.reference);

    // Correct: 25%
    ASSERT_TRUE(escFlameIsItPackageStart(pwm_25.raw));
    escFlameParse(pwm_25.raw, &esc_status);
    ASSERT_FB_EQUAL(esc_status, pwm_25.reference);

    // // Correct: 55%
    ASSERT_TRUE(escFlameIsItPackageStart(pwm_55.raw));
    escFlameParse(pwm_55.raw, &esc_status);
    ASSERT_FB_EQUAL(esc_status, pwm_55.reference);
}

TEST(EscFlame, escFlameParseDma_wrong_args) {
    uint8_t buffer[96];
    DmaUartHandler_t parser = {
        .buf = buffer,
        .size = 96,
    };
    EscFlame_t esc_status;
    size_t recv_idx = 0;

    ASSERT_FALSE(escFlameParseDma(parser.size, &parser, &esc_status));
    ASSERT_FALSE(escFlameParseDma(recv_idx, NULL, &esc_status));
    ASSERT_FALSE(escFlameParseDma(recv_idx, &parser, NULL));
}

TEST(EscFlame, escFlameParseDma_empty) {
    uint8_t buffer[96];
    DmaUartHandler_t parser = {
        .buf = buffer,
        .size = 96,
    };
    EscFlame_t esc_status;
    size_t recv_idx = 0;

    escFlameParseDma(recv_idx, &parser, &esc_status);
}

TEST(EscFlame, escFlameParseDma_perfect_sequence) {
    uint8_t buffer[96];
    DmaUartHandler_t parser = {
        .buf = buffer,
        .size = 96,
    };
    EscFlame_t esc_status;

    for (size_t package_idx = 0; package_idx < 9; package_idx++) {
        size_t parser_buf_idx = (package_idx * ESC_FLAME_PACKAGE_SIZE) % parser.size;
        size_t recv_idx = (parser_buf_idx + ESC_FLAME_PACKAGE_SIZE - 1) % parser.size;

        const uint8_t* real_case;
        if (package_idx % 3 == 0) {
            real_case = pwm_0.raw;
        } else if (package_idx % 3 == 1) {
            real_case = pwm_7.raw;
        } else if (package_idx % 3 == 2) {
            real_case = pwm_20.raw;
        }

        fill_buffer_with_frame(parser, parser_buf_idx, real_case);

        ASSERT_TRUE(escFlameParseDma(recv_idx, &parser, &esc_status));
        ASSERT_EQ(parser.saved_idx, recv_idx);

        if (package_idx % 3 == 0) {
            ASSERT_FB_EQUAL(esc_status, pwm_0.reference);
        } else if (package_idx % 3 == 1) {
            ASSERT_FB_EQUAL(esc_status, pwm_7.reference);
        } else if (package_idx % 3 == 2) {
            ASSERT_FB_EQUAL(esc_status, pwm_20.reference);
        }
    }
}

TEST(EscFlame, escFlameParseDma_offset_sequence) {
    uint8_t buffer[96];
    DmaUartHandler_t parser = {
        .buf = buffer,
        .size = 96,
    };
    EscFlame_t esc_status;

    for (size_t package_idx = 0; package_idx < 4; package_idx++) {
        size_t parser_buf_idx = (12 + package_idx * ESC_FLAME_PACKAGE_SIZE) % parser.size;
        size_t recv_idx = (parser_buf_idx + ESC_FLAME_PACKAGE_SIZE - 1) % parser.size;

        const uint8_t* real_case;
        if (package_idx % 3 == 0) {
            real_case = pwm_0.raw;
        } else if (package_idx % 3 == 1) {
            real_case = pwm_7.raw;
        } else if (package_idx % 3 == 2) {
            real_case = pwm_20.raw;
        }

        fill_buffer_with_frame(parser, parser_buf_idx, real_case);

        ASSERT_TRUE(escFlameParseDma(recv_idx, &parser, &esc_status));
        ASSERT_EQ(parser.saved_idx, recv_idx);

        if (package_idx % 3 == 0) {
            ASSERT_FB_EQUAL(esc_status, pwm_0.reference);
        } else if (package_idx % 3 == 1) {
            ASSERT_FB_EQUAL(esc_status, pwm_7.reference);
        } else if (package_idx % 3 == 2) {
            ASSERT_FB_EQUAL(esc_status, pwm_20.reference);
        }

        size_t intermediate_idx;
        intermediate_idx = (recv_idx + 0) % parser.size;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
        intermediate_idx = (recv_idx + 1) % parser.size;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
        intermediate_idx = (recv_idx + 2) % parser.size;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
    }
}

TEST(EscFlame, escFlameParseDma_poll_faster_than_measure) {
    uint8_t buffer[96];
    DmaUartHandler_t parser = {
        .buf = buffer,
        .size = 96,
    };
    EscFlame_t esc_status;

    for (size_t package_idx = 0; package_idx < 9; package_idx++) {
        size_t parser_buf_idx = (package_idx * ESC_FLAME_PACKAGE_SIZE) % parser.size;
        size_t recv_idx = (parser_buf_idx + ESC_FLAME_PACKAGE_SIZE - 1) % parser.size;

        const uint8_t* real_case;
        if (package_idx % 3 == 0) {
            real_case = pwm_0.raw;
        } else if (package_idx % 3 == 1) {
            real_case = pwm_7.raw;
        } else if (package_idx % 3 == 2) {
            real_case = pwm_20.raw;
        }

        fill_buffer_with_frame(parser, parser_buf_idx, real_case);

        ASSERT_TRUE(escFlameParseDma(recv_idx, &parser, &esc_status));
        ASSERT_EQ(parser.saved_idx, recv_idx);

        if (package_idx % 3 == 0) {
            ASSERT_FB_EQUAL(esc_status, pwm_0.reference);
        } else if (package_idx % 3 == 1) {
            ASSERT_FB_EQUAL(esc_status, pwm_7.reference);
        } else if (package_idx % 3 == 2) {
            ASSERT_FB_EQUAL(esc_status, pwm_20.reference);
        }

        size_t intermediate_idx;
        intermediate_idx = (recv_idx + 0) % parser.size;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
        intermediate_idx = (recv_idx + 1) % parser.size;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
        intermediate_idx = (recv_idx + 2) % parser.size;
        ASSERT_FALSE(escFlameParseDma(intermediate_idx, &parser, &esc_status));
    }
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
