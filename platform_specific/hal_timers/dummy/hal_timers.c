/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_timers.h"
#include "libperiph_common.h"

int8_t timerInit(Channel_t channel, TimerMode_t mode) {
    return STATUS_OK;
}

TimerMode_t timerGetMode(Channel_t channel) {
    return TIMER_NOT_CONFIGURED;
}

void timerSetPwmFrequency(Channel_t ch, uint16_t frequency_hz) {
}

void timerSetPwmDuration(Channel_t channel, uint32_t pwm) {
}

int32_t timerGetPwmDuration(Channel_t channel) {
    return 1000;
}

uint32_t timerGetCapturedValue(Channel_t channel) {
    return 0;
}
