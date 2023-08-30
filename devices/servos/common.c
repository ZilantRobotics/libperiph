/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "common.h"


static float clampFloat(float value, float first, float second);
static float maxFloat(float first, float second);
static float minFloat(float first, float second);


static const RawCommand_t RC_MIN = 0;
static const RawCommand_t RC_MAX = 8191;
PwmDurationMillisecond_t mapRawCommandToPwm(RawCommand_t rc_value,
                                            PwmDurationMillisecond_t min_pwm,
                                            PwmDurationMillisecond_t max_pwm,
                                            PwmDurationMillisecond_t def_pwm) {
    PwmDurationMillisecond_t pwm;
    if (rc_value < RC_MIN || rc_value > RC_MAX) {
        pwm = def_pwm;
    } else {
        pwm = (PwmDurationMillisecond_t)mapFloat(rc_value, RC_MIN, RC_MAX, min_pwm, max_pwm);
    }
    return pwm;
}

float minFloat(float first, float second) {
    return (first < second) ? first : second;
}

float maxFloat(float first, float second) {
    return (first > second) ? first : second;
}

float clampFloat(float value, float first, float second) {
    float min_value = minFloat(first, second);
    float max_value = maxFloat(first, second);

    if (value <= min_value) {
        return min_value;
    } else if (value >= max_value) {
        return max_value;
    } else {
        return value;
    }
}

float mapFloat(float value, float in_min, float in_max, float out_min, float out_max) {
    float output;
    if (value <= in_min && in_min <= in_max) {
        output = out_min;
    } else if (value >= in_max && in_min <= in_max) {
        output = out_max;
    } else if (out_min == out_max) {
        output = out_min;
    } else {
        output = out_min + (value - in_min) / (in_max - in_min) * (out_max - out_min);
        output = clampFloat(output, out_min, out_max);
    }
    return output;
}
