/*
 * Copyright (C) 2019-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file rgb_led.h
 * @author d.ponomarev
 * @date Dec 07, 2022
 */

#ifndef SRC_DRIVERS_INTERNAL_LED_RGB_RGB_LED_H_
#define SRC_DRIVERS_INTERNAL_LED_RGB_RGB_LED_H_

#include <stdint.h>

typedef enum {
    RGB_LED_SOLID_BLUE,
    RGB_LED_SOLID_GREEN,
    RGB_LED_BLINKING_BLUE,
    RGB_LED_BLINKING_GREEN,
    RGB_LED_BLINKING_YELLOW,
    RGB_LED_BLINKING_RED,
    RGB_LED_BLINKING_RED_BLUE,
    RGB_LED_UNUSED,
} RgbLedState_t;


void rgbLedUpdateState(RgbLedState_t rgb_led_state, uint32_t crnt_time_ms);

#endif  // SRC_DRIVERS_INTERNAL_LED_RGB_RGB_LED_H_
