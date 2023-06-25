/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef DEVICES_SERVOS_COMMON_H_
#define DEVICES_SERVOS_COMMON_H_

#include <stdbool.h>
#include <stdint.h>
#include "hal_timers.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef uint16_t PwmDurationMillisecond_t;
typedef int16_t RawCommand_t;
/**
  * @brief  Map raw command value (in interval from 0 to 8191)
  *         to PWM duration (in interval from min to max)
  * @return pwm_duration if input is correct,
  *         def_pwm if raw_command value is less than min or higher than max
  */
PwmDurationMillisecond_t mapRawCommandToPwm(RawCommand_t rc_value,
                                            PwmDurationMillisecond_t min_pwm,
                                            PwmDurationMillisecond_t max_pwm,
                                            PwmDurationMillisecond_t def_pwm);

float mapFloat(float value, float in_min, float in_max, float out_min, float out_max);


#ifdef __cplusplus
}
#endif

#endif  // DEVICES_SERVOS_COMMON_H_
