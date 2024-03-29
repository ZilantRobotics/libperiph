/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file servos.c
 * @author d.ponomarev
 * @date Aug 11, 2022
 */

#include "servos.h"
#include "ttl.h"
#include "libperiph_common.h"

#define DEFAULT_SETPOINT_VALUE -1

static ServoParameters_t params[SERVO_TIM_CHANNELS_AMOUNT];
static bool inited_channels[SERVO_TIM_CHANNELS_AMOUNT] = {};
static int16_t setpoints[SETPOINTS_AMOUNT] = {};  ///< the same as RawCommand
static uint32_t arm_ts_ms = 0;

static int8_t servosInitPwmChannel(Channel_t tim_channel_idx);
static void servosSetDefaultValueForChannel(Channel_t tim_channel);
static void servosUpdateChannelStateAccordingToSetpoint(Channel_t tim_ch);
static void servosProcessTimeToLiveChecks(uint32_t crnt_ts_ms);

int8_t servosInitChannel(Channel_t tim_channel, const ServoParameters_t* servo_params) {
    if (!servo_params || (uint32_t)tim_channel >= SERVO_TIM_CHANNELS_AMOUNT) {
        return STATUS_ERROR;
    }

    if (servo_params->ch < SETPOINTS_AMOUNT) {
        setpoints[servo_params->ch] = DEFAULT_SETPOINT_VALUE;
    }

    servosUpdateParams(tim_channel, servo_params);

    if (inited_channels[tim_channel]) {
        return STATUS_OK;
    }

    if (servosInitPwmChannel(tim_channel) == STATUS_ERROR) {
        return STATUS_ERROR;
    }
    inited_channels[tim_channel] = true;

    return STATUS_OK;
}

void servosUpdateParams(Channel_t tim_ch_idx, const ServoParameters_t* new_params) {
    if (!new_params || tim_ch_idx >= SERVO_TIM_CHANNELS_AMOUNT) {
        return;
    }
    params[tim_ch_idx].ch = new_params->ch;
    params[tim_ch_idx].min = new_params->min;
    params[tim_ch_idx].max = new_params->max;
    params[tim_ch_idx].def = new_params->def;
}

void servosSetTimeout(uint32_t ttl_ms) {
    ttlSetTimeout(ttl_ms);
}

void servosSetArmingState(bool arm, uint32_t crnt_time_ms) {
    arm_ts_ms = arm ? crnt_time_ms : 0;
}

void servosSetSetpoint(uint8_t sp_idx, int16_t value, uint32_t crnt_time_ms) {
    if (sp_idx < SETPOINTS_AMOUNT) {
        setpoints[sp_idx] = value;
        ttlSetSetpointTimestamp(sp_idx, crnt_time_ms);
    }
}

void servosApplyPwm(uint32_t crnt_ts_ms) {
    servosProcessTimeToLiveChecks(crnt_ts_ms);
    for (uint_fast8_t tim_idx = 0; tim_idx < SERVO_TIM_CHANNELS_AMOUNT; tim_idx++) {
        Channel_t tim_ch = (Channel_t)tim_idx;

        if (arm_ts_ms + ttlGetTimeout() < crnt_ts_ms) {
            servosSetDefaultValueForChannel(tim_ch);
        } else {
            servosUpdateChannelStateAccordingToSetpoint(tim_ch);
        }
    }
}

bool servosIsChannelInited(Channel_t tim_ch) {
    bool is_tim_idx_ok = tim_ch < SERVO_TIM_CHANNELS_AMOUNT;
    bool is_sp_idx_ok = params[tim_ch].ch < SETPOINTS_AMOUNT;
    bool is_mode_ok = timerGetMode(tim_ch) == TIMER_MODE_PWM;
    return is_tim_idx_ok && is_sp_idx_ok && inited_channels[tim_ch] && is_mode_ok;
}

int8_t servosGetPwmPercent(Channel_t tim_ch) {
    int32_t percent;
    if (tim_ch < SERVO_TIM_CHANNELS_AMOUNT && timerGetMode(tim_ch) == TIMER_MODE_PWM) {
        uint32_t pwm = (uint32_t)timerGetPwmDuration(tim_ch);
        percent = (int32_t)mapFloat(pwm, params[tim_ch].min, params[tim_ch].max, 0, 100);
    } else {
        percent = STATUS_ERROR;
    }
    return percent;
}

uint32_t servosGetTimerSetpointIndex(Channel_t tim_ch) {
    return params[tim_ch].ch;
}

Channel_t servosGetTimerChannelBySetpointChannel(uint32_t sp_ch) {
    for (uint_fast8_t tim_idx = 0; tim_idx < SERVO_TIM_CHANNELS_AMOUNT; tim_idx++) {
        Channel_t tim_ch = (Channel_t)tim_idx;
        if (params[tim_ch].ch == sp_ch) {
            return tim_ch;
        }
    }
    return TIM_CH_AMOUNT;
}

int16_t servosGetSetpoint(uint8_t sp_idx) {
    return (sp_idx < SETPOINTS_AMOUNT) ? setpoints[sp_idx] : -1;
}

bool servosGetArmingState(uint32_t crnt_time_ms) {
    return arm_ts_ms != 0 && arm_ts_ms + ttlGetTimeout() >= crnt_time_ms;
}


bool servosGetEstimatedArmStatus(uint32_t crnt_time_ms) {
    uint32_t ttl_ms = ttlGetTimeout();
    if (ttlGetBestTimestamp() + ttl_ms < crnt_time_ms) {
        return false;
    }
    for (uint_fast8_t sp_idx = 0; sp_idx < SETPOINTS_AMOUNT; sp_idx++) {
        if (servosGetSetpoint(sp_idx) > 0) {
            return true;
        }
    }
    return false;
}

///< *************************** PRIVATE FUNCTIONS ***************************
/**
 * @note All args checks must be done out of here scope
 */
static int8_t servosInitPwmChannel(Channel_t tim_channel_idx) {
    int8_t status;
    if (timerInit(tim_channel_idx, TIMER_MODE_PWM) != STATUS_OK) {
        status = STATUS_ERROR;
    } else {
        servosSetDefaultValueForChannel(tim_channel_idx);
        status = STATUS_OK;
    }
    return status;
}

static void servosSetDefaultValueForChannel(Channel_t tim_ch) {
    timerSetPwmDuration(tim_ch, params[tim_ch].def);
}

static void servosUpdateChannelStateAccordingToSetpoint(Channel_t tim_ch) {
    uint8_t sp_idx = (uint8_t)servosGetTimerSetpointIndex(tim_ch);
    RawCommand_t val = servosGetSetpoint(sp_idx);
    PwmDurationMillisecond_t min = params[tim_ch].min;
    PwmDurationMillisecond_t max = params[tim_ch].max;
    PwmDurationMillisecond_t def = params[tim_ch].def;

    timerSetPwmDuration(tim_ch, mapRawCommandToPwm(val, min, max, def));
}


static void servosProcessTimeToLiveChecks(uint32_t crnt_ts_ms) {
    for (uint_fast8_t tim_idx = 0; tim_idx < SERVO_TIM_CHANNELS_AMOUNT; tim_idx++) {
        uint8_t sp_idx = params[tim_idx].ch;
        if (sp_idx >= SETPOINTS_AMOUNT) {
            continue;
        }
        if (!ttlIsSetpointAlive(sp_idx, crnt_ts_ms)) {
            setpoints[sp_idx] = DEFAULT_SETPOINT_VALUE;
        }
    }
}
