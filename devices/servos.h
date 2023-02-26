/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file servos.h
 * @author d.ponomarev
 * @date Aug 11, 2022
 */

#ifndef SRC_DRIVERS_INC_SERVOS_H_
#define SRC_DRIVERS_INC_SERVOS_H_

#include <stdbool.h>
#include <stdint.h>
#include "hal_timers.h"


#define SERVO_TIM_CHANNELS_AMOUNT   9
#define SETPOINTS_AMOUNT            20


typedef enum {
    SETPOINT_RAW_COMMAND,
    SETPOINT_ARRAY_COMMAND,
} SetpointType_t;

typedef struct {
    uint32_t ch;
    uint32_t min;
    uint32_t max;
    uint32_t def;
} ServoParameters_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  Initialize the given timer channel in PWM mode.
  * @return either STATUS_OK or STATUS_ERROR depending on success of initialization.
  */
int8_t uavcanServosInitChannel(Channel_t tim_channel, const ServoParameters_t* servo_params);
void uavcanServosUpdateParams(Channel_t tim_ch, const ServoParameters_t* new_params);

/**
  * @brief You should periodically call this function to set default values to `dead` setpoints.
  * uavcanServosSetSetpoint and uavcanServosProcessTimeToLiveChecks do not actually update the output
  * uavcanServosUpdateAllChannelsPwm updates the actual output pin state
  */
void uavcanServosSetArmingStatus(bool arm, uint32_t crnt_time_ms);
void uavcanServosSetSetpoint(uint8_t sp_idx, int16_t value, uint32_t crnt_time_ms);
void uavcanServosProcessTimeToLiveChecks(uint32_t crnt_ts_ms);
void uavcanServosUpdateAllChannelsPwm(uint32_t crnt_ts_ms);

/**
  * @return Auxillary getters
  */
bool uavcanServosIsChannelInited(Channel_t tim_ch);
int8_t uavcanServosGetPwmPercent(Channel_t tim_ch);
uint32_t uavcanServosGetTimerSetpoint(Channel_t tim_ch);
Channel_t uavcanServosGetTimerChannelBySetpointChannel(uint32_t sp_ch);
int16_t uavcanServosGetSetpoint(uint8_t sp_idx);
bool uavcanServosGetEstimatedArmStatus(uint32_t crnt_time_ms);


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

#endif  // SRC_DRIVERS_INC_SERVOS_H_
