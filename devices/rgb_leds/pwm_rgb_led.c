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
#define RED_TIMER_CHANNEL   PIN_PB5_TIM3_CH2
#define GREEN_TIMER_CHANNEL PIN_PB0_TIM3_CH3
#define BLUE_TIMER_CHANNEL  PIN_PB4_TIM3_CH1


int8_t ledRgbInit() {
    if (timerInit(PIN_PB4_TIM3_CH1, TIMER_MODE_PWM) == STATUS_ERROR ||
            timerInit(PIN_PB5_TIM3_CH2, TIMER_MODE_PWM) == STATUS_ERROR ||
            timerInit(PIN_PB0_TIM3_CH3, TIMER_MODE_PWM) == STATUS_ERROR) {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

void ledRgbSetColors(Color_t color) {
    timerSetPwmDuration(RED_TIMER_CHANNEL, color.shades.red);
    timerSetPwmDuration(GREEN_TIMER_CHANNEL, color.shades.green);
    timerSetPwmDuration(BLUE_TIMER_CHANNEL, color.shades.blue);
}
