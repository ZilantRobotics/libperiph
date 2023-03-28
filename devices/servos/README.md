# Servo driver

## Requirements for the driver

The physical channel should be in 2 states:
- default (when disarmed or there is no command for a specific period of time)
- engaged where the the PWM value is between MIN and MAX values based on the setpoint value.

Multiple setpoint and physical channels should be supported.

API should be divided into 3 parts:
- setting setpoint value for a specific channel,
- setting readiness (arm) state for all channels,
- applying the new setpoint value and readiness state.

## Prerequisites

Actual timer driver should be implemented outside.

## Usage example:

Typically you need to write 3 functions for setpoint and readiness callback and for periodical process.

```c++
#include "servo.h"

void application_init() {
    ...
    Channel_t tim_ch = PIN_PB7_TIM4_CH2;
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };

    servosInitChannel(tim_ch, &params);
    servosSetTimeout(500);
    ...
}

void setpoint_callback() {
    uint8_t sp_idx = 0;
    int16_t value = 4095; // 50%
    uint32_t crnt_time_ms = HAL_GetTick();

    servosSetSetpoint(sp_idx, setpoint_value, crnt_time_ms);
    servosApplyPwm(crnt_time_ms);
}

void readiness_callback(bool arm) {
    uint32_t crnt_time_ms = HAL_GetTick();

    servosSetArmingState(arm, crnt_time_ms);
    servosApplyPwm(crnt_time_ms);
}

void application_periodical_process() {
    uint32_t crnt_time_ms = HAL_GetTick();
    servosApplyPwm(crnt_time_ms);
}

```
