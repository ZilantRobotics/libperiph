/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>
#include <iostream>
#include "rangefinder/tf_mini/tf_mini.h"

uint8_t frame_ok_1[] =   {'Y', 'Y', 53, 0, 25, 0, 3,   16,  19};
uint8_t frame_ok_2[] =   {'Y', 'Y', 0,  0, 0,  0, 205, 55,  182};
uint8_t frame_ok_3[] =   {'Y', 'Y', 0,  0, 0,  0, 128, 140, 190};
uint8_t frame_ok_4[] =   {'Y', 'Y', 0,  0, 0,  0, 4,   148, 74};

uint8_t frame_ok_5_1[] = {'Y', 'Y', 95, 0, 96, 1, 140, 95,  93};
uint8_t frame_ok_5_2[] = {'Y', 'Y', 95, 0, 95, 1, 168, 103, 128};

uint8_t frame_ok_6_1[] = {95, 0, 96, 1, 140, 95,  93, 'Y', 'Y'};
uint8_t frame_ok_6_2[] = {95, 0, 95, 1, 168, 103, 128, 'Y', 'Y', };

TEST(tf_mini, test_tfMiniInit) {
    ASSERT_EQ(LIBPERIPH_OK, tfMiniInit());
}

TEST(tf_mini, test_tfParseRange) {
    ASSERT_FLOAT_EQ(0.53, tfParseRange((const TfMiniSerialFrame_t*)frame_ok_1));
    ASSERT_FLOAT_EQ(0.0, tfParseRange((const TfMiniSerialFrame_t*)frame_ok_2));
    ASSERT_FLOAT_EQ(0.0, tfParseRange((const TfMiniSerialFrame_t*)frame_ok_3));
    ASSERT_FLOAT_EQ(0.0, tfParseRange((const TfMiniSerialFrame_t*)frame_ok_4));
    ASSERT_FLOAT_EQ(0.95, tfParseRange((const TfMiniSerialFrame_t*)frame_ok_5_1));
    ASSERT_FLOAT_EQ(0.95, tfParseRange((const TfMiniSerialFrame_t*)frame_ok_5_2));

    tfParseRange((const TfMiniSerialFrame_t*)frame_ok_6_1);
    ASSERT_FLOAT_EQ(0.95, tfParseRange((const TfMiniSerialFrame_t*)frame_ok_6_2));

    // Bad argument
    ASSERT_FLOAT_EQ(-1.0f, tfParseRange(NULL));
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
