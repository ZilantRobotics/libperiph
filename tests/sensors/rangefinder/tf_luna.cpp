/**
 * @file tf_luna.cpp
 * @author d.ponomarev
 * @date Jun 05, 2021
 */

#include <gtest/gtest.h>
#include <iostream>
#include "rangefinder/tf_luna/tf_luna.h"

uint8_t frame_ok_1[] =   {'Y', 'Y', 53, 0, 25, 0, 3,   16,  19};
uint8_t frame_ok_2[] =   {'Y', 'Y', 0,  0, 0,  0, 205, 55,  182};
uint8_t frame_ok_3[] =   {'Y', 'Y', 0,  0, 0,  0, 128, 140, 190};
uint8_t frame_ok_4[] =   {'Y', 'Y', 0,  0, 0,  0, 4,   148, 74};

uint8_t frame_ok_5_1[] = {'Y', 'Y', 95, 0, 96, 1, 140, 95,  93};
uint8_t frame_ok_5_2[] = {'Y', 'Y', 95, 0, 95, 1, 168, 103, 128};

uint8_t frame_ok_6_1[] = {95, 0, 96, 1, 140, 95,  93, 'Y', 'Y'};
uint8_t frame_ok_6_2[] = {95, 0, 95, 1, 168, 103, 128, 'Y', 'Y', };


TEST(TfLuna, parser_ok_1) {
    ASSERT_FLOAT_EQ(0.53, tfParseRange((const TfLunaSerialFrame_t*)frame_ok_1));
}

TEST(TfLuna, parser_ok_2) {
    ASSERT_FLOAT_EQ(0.0, tfParseRange((const TfLunaSerialFrame_t*)frame_ok_2));
}

TEST(TfLuna, parser_ok_3) {
    ASSERT_FLOAT_EQ(0.0, tfParseRange((const TfLunaSerialFrame_t*)frame_ok_3));
}

TEST(TfLuna, parser_ok_4) {
    ASSERT_FLOAT_EQ(0.0, tfParseRange((const TfLunaSerialFrame_t*)frame_ok_4));
}

TEST(TfLuna, parser_ok_5) {
    ASSERT_FLOAT_EQ(0.95, tfParseRange((const TfLunaSerialFrame_t*)frame_ok_5_1));
    ASSERT_FLOAT_EQ(0.95, tfParseRange((const TfLunaSerialFrame_t*)frame_ok_5_2));
}

TEST(TfLuna, parser_ok_6) {
    tfParseRange((const TfLunaSerialFrame_t*)frame_ok_6_1);
    ASSERT_FLOAT_EQ(0.95, tfParseRange((const TfLunaSerialFrame_t*)frame_ok_6_2));
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}