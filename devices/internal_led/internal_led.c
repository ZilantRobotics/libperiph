/*
 * Copyright (C) 2019-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file internal_led.c
 * @author d.ponomarev
 * @date Dec 07, 2022
 */

#include "internal_led.h"
#include "stdint.h"
#include "rgb/rgb_led.h"
#include "legacy/legacy_led.h"
#include "main.h"


#ifndef INTERNAL_LED_LEGACY
    #define INTERNAL_LED_LEGACY 0
#endif

#ifndef INTERNAL_LED_RGB
    #define INTERNAL_LED_RGB 0
#endif


void internalLedSetState(InternalLedState_t inernal_led_state) {
    RgbLedState_t rgb_led_state = RGB_LED_UNUSED;
    uint8_t num_of_pulses = 0;

    switch (inernal_led_state) {
        case INTERNAL_LED_STATE_ARMED_NO_FIX:
            rgb_led_state = RGB_LED_SOLID_BLUE;
            num_of_pulses = 1;
            break;

        case INTERNAL_LED_STATE_ARMED_3D_FIX:
            rgb_led_state = RGB_LED_SOLID_GREEN;
            num_of_pulses = 1;
            break;

        case INTERNAL_LED_STATE_DISARMED_NO_FIX:
            rgb_led_state = RGB_LED_BLINKING_BLUE;
            num_of_pulses = 2;
            break;

        case INTERNAL_LED_STATE_DISARMED_3D_FIX:
            rgb_led_state = RGB_LED_BLINKING_GREEN;
            num_of_pulses = 2;
            break;

        case INTERNAL_LED_STATE_DISARMED_WARNING:
            rgb_led_state = RGB_LED_BLINKING_YELLOW;
            num_of_pulses = 3;
            break;

        case INTERNAL_LED_STATE_ERROR:
        default:
            rgb_led_state = RGB_LED_BLINKING_RED;
            num_of_pulses = 5;
            break;
    }

#if INTERNAL_LED_RGB == 1
    uint32_t crnt_time_ms = HAL_GetTick();
    rgbLedUpdateState(rgb_led_state, crnt_time_ms);
#else
    (void)rgb_led_state;
#endif

#if INTERNAL_LED_LEGACY == 1
    ledPulseSequence(num_of_pulses);
#else
    (void)num_of_pulses;
#endif
}
