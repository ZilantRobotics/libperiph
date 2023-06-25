/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "rgb_leds/ws2812.h"

int8_t HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef*, uint8_t, uint32_t*, uint32_t) {
    return 0;
}
int8_t HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef*, uint8_t) {
    return 0;
}

TEST(ws2812, ws2812) {
    ws2812bInit(4, NULL, 0);
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
