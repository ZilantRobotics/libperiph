/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file pwm_rgb_led.c
 * @author d.ponomarev
 * @date Nov 08, 2021
 * @note
 */

#include "pwm_rgb_led.h"
#include "config.h"
#include "hal_timers.h"


// BRG
#define RED_TIMER_CHANNEL   TIM_3_CH_2_PIN_PB5
#define GREEN_TIMER_CHANNEL TIM_3_CH_3_PIN_PB0
#define BLUE_TIMER_CHANNEL  TIM_3_CH_1_PIN_PB4


int8_t ledRgbInit() {
    if (timerInit(TIM_3_CH_1_PIN_PB4, TIMER_MODE_PWM) == STATUS_ERROR ||
            timerInit(TIM_3_CH_2_PIN_PB5, TIMER_MODE_PWM) == STATUS_ERROR ||
            timerInit(TIM_3_CH_3_PIN_PB0, TIMER_MODE_PWM) == STATUS_ERROR) {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

void ledRgbSetColors(Color_t color) {
    timerSetPwmDuration(RED_TIMER_CHANNEL, color.shades.red);
    timerSetPwmDuration(GREEN_TIMER_CHANNEL, color.shades.green);
    timerSetPwmDuration(BLUE_TIMER_CHANNEL, color.shades.blue);
}
