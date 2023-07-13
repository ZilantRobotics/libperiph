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
#include <iostream>

// Overloaded version of weak ubuntuStm32TimerCallback()
void ubuntuStm32TimerCallback(Channel_t ch) {
    printf("Update timer: idx=%i, mode=%i, duration=%i\n",
           ch,
           timerGetMode(ch),
           timerGetPwmDuration(ch));
}

TEST(hal_timers, test_timerInit) {
    ASSERT_EQ(LIBPERIPH_OK, timerInit(PIN_PB7_TIM4_CH2, TIMER_MODE_PWM));

    ASSERT_EQ(-HAL_TIMERS_WRONG_ARG, timerInit(TIM_CH_AMOUNT, TIMER_MODE_PWM));
    ASSERT_EQ(-HAL_TIMERS_WRONG_ARG, timerInit(PIN_PB7_TIM4_CH2, TIMER_MODES_AMOUNT));
}

TEST(hal_timers, test_timerGetMode) {
    auto channel = PIN_PB7_TIM4_CH2;

    ASSERT_EQ(LIBPERIPH_OK, timerInit(channel, TIMER_MODE_PWM));
    ASSERT_EQ(TIMER_MODE_PWM, timerGetMode(channel));

    ASSERT_EQ(LIBPERIPH_OK, timerInit(channel, TIMER_MODE_CAPTURE));
    ASSERT_EQ(TIMER_MODE_CAPTURE, timerGetMode(channel));

    ASSERT_EQ(TIMER_NOT_CONFIGURED, timerGetMode(TIM_CH_AMOUNT));
}

TEST(hal_timers, test_timerGetSetPwmDuration) {
    auto channel = PIN_PB7_TIM4_CH2;

    ASSERT_EQ(LIBPERIPH_OK, timerInit(channel, TIMER_MODE_PWM));
    timerSetPwmDuration(channel, 1000);
    ASSERT_EQ(1000, timerGetPwmDuration(channel));

    ASSERT_EQ(LIBPERIPH_OK, timerInit(channel, TIMER_MODE_PWM));
    timerSetPwmDuration(channel, 2000);
    ASSERT_EQ(2000, timerGetPwmDuration(channel));
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
