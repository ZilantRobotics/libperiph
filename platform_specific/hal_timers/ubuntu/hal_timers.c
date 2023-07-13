/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_timers.h"
#include <stdio.h>
#include "libperiph_common.h"


typedef struct {
    const char* name;
    uint32_t duration;
    uint32_t frequency;
    TimerMode_t mode;
} TimChannel_t;

static volatile TimChannel_t channels[TIM_CH_AMOUNT] = {
    {.name = "PIN_PB7_TIM4_CH2", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},
    {.name = "PIN_PB6_TIM4_CH1", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},

    {.name = "PIN_A3_TIM2_CH4", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},
    {.name = "PIN_A2_TIM2_CH3", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},

    {.name = "PIN_A9_TIM1_CH2", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},
    {.name = "PIN_A8_TIM1_CH1", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},
    {.name = "PIN_A10_TIM1_CH3", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},

    {.name = "PIN_PB4_TIM3_CH1", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},
    {.name = "PIN_PB5_TIM3_CH2", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},
    {.name = "PIN_PB0_TIM3_CH3", .duration = 0, .frequency = 50, .mode = TIMER_NOT_CONFIGURED},
};

static const char* getChannelName(Channel_t ch);

__attribute__((weak)) void ubuntuStm32TimerCallback(Channel_t ch) {
    printf("Update %s: idx=%i, mode=%i, frequency=%i, duration=%i\n",
           getChannelName(ch),
           ch,
           timerGetMode(ch),
           channels[ch].frequency,
           timerGetPwmDuration(ch));
}

int8_t timerInit(Channel_t ch, TimerMode_t mode) {
    int8_t res;

    if (mode >= TIMER_MODES_AMOUNT || ch >= TIM_CH_AMOUNT) {
        res = -HAL_TIMERS_WRONG_ARG;
    } else {
        channels[ch].mode = mode;
        res = STATUS_OK;
    }

    ubuntuStm32TimerCallback(ch);

    return res;
}

TimerMode_t timerGetMode(Channel_t ch) {
    return (ch >= TIM_CH_AMOUNT) ? TIMER_NOT_CONFIGURED : channels[ch].mode;
}

void timerSetPwmFrequency(Channel_t ch, uint16_t frequency_hz) {
    if (ch >= TIM_CH_AMOUNT || channels[ch].mode != TIMER_MODE_PWM) {
        return;
    }

    channels[ch].frequency = frequency_hz;
    ubuntuStm32TimerCallback(ch);
}

void timerSetPwmDuration(Channel_t ch, uint32_t pwm) {
    if (ch >= TIM_CH_AMOUNT || channels[ch].mode != TIMER_MODE_PWM) {
        return;
    }

    if (channels[ch].duration != pwm) {
        channels[ch].duration = pwm;
        ubuntuStm32TimerCallback(ch);
    }
}

int32_t timerGetPwmDuration(Channel_t ch) {
    if (ch >= TIM_CH_AMOUNT || channels[ch].mode != TIMER_MODE_PWM) {
        return 0;
    }

    return channels[ch].duration;
}

uint32_t timerGetCapturedValue(Channel_t ch) {
    return 0;
}

const char* getChannelName(Channel_t ch) {
    if (ch >= TIM_CH_AMOUNT) {
        return "Unknown";
    } else {
        return channels[ch].name;
    }
}
