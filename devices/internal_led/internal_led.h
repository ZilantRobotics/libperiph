/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file internal_led.h
 * @author d.ponomarev
 * @date Dec 07, 2022
 */

#ifndef SRC_DRIVERS_INTERNAL_LED_INTERNAL_LED_H_
#define SRC_DRIVERS_INTERNAL_LED_INTERNAL_LED_H_

typedef enum {
    INTERNAL_LED_STATE_ARMED_NO_FIX,        // Solid Blue
    INTERNAL_LED_STATE_ARMED_3D_FIX,        // Solid Green
    INTERNAL_LED_STATE_DISARMED_NO_FIX,     // Blinking Blue
    INTERNAL_LED_STATE_DISARMED_3D_FIX,     // Blinking Green
    INTERNAL_LED_STATE_DISARMED_WARNING,    // Blinking Yellow
    INTERNAL_LED_STATE_ERROR,               // Blinking Red
    INTERNAL_LED_STATE_INIT,                // Blinking Red/Blue
} InternalLedState_t;


void internalLedSetState(InternalLedState_t inernal_led_state);

#endif  // SRC_DRIVERS_INTERNAL_LED_INTERNAL_LED_H_
