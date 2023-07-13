/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "hal_timers.h"
#include "libperiph_common.h"


TEST(hal_timers, test) {
    ASSERT_EQ(LIBPERIPH_OK, timerInit(PIN_PB7_TIM4_CH2, TIMER_MODE_CAPTURE));
    ASSERT_EQ(TIMER_NOT_CONFIGURED, timerGetMode(PIN_PB7_TIM4_CH2));
    ASSERT_EQ(1000, timerGetPwmDuration(PIN_PB7_TIM4_CH2));
    ASSERT_EQ(0, timerGetCapturedValue(PIN_PB7_TIM4_CH2));
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
