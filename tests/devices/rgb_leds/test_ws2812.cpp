/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "rgb_leds/ws2812.h"
#include "rgb_leds/rgb_color.h"

int8_t HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef*, uint32_t, uint32_t*, uint32_t) {
    return 0;
}
int8_t HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef*, uint32_t) {
    return 0;
}

TEST(ws2812, test_ws2812bInit) {
    TIM_HandleTypeDef tim;

    ASSERT_EQ(LIBPERIPH_OK, ws2812bInit(4, &tim, 0));
    ASSERT_EQ(LIBPERIPH_ERROR, ws2812bInit(MAX_NUM_OF_LEDS + 1, &tim, 0));
    ASSERT_EQ(LIBPERIPH_ERROR, ws2812bInit(4, NULL, 0));
}

TEST(ws2812, test_ws2812bSetColors) {
    TIM_HandleTypeDef tim;
    Leds_Color_t leds_colors;

    ASSERT_EQ(LIBPERIPH_OK, ws2812bInit(4, &tim, 0));
    ws2812bSetColors(&leds_colors);
    ASSERT_EQ(LIBPERIPH_OK, ws2812bStartOnce());

    ASSERT_EQ(LIBPERIPH_ERROR, ws2812bInit(4, NULL, 0));
    ws2812bSetColors(&leds_colors);
    ASSERT_EQ(LIBPERIPH_ERROR, ws2812bStartOnce());
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
