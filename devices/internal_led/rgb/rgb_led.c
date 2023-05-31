/*
 * Copyright (C) 2019-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file rgb_led.c
 * @author d.ponomarev
 * @date Dec 07, 2022
 */

#include "rgb_led.h"
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

void redLedSetState(bool led_state) {
    GPIO_PinState pin_state = (led_state == true) ? GPIO_PIN_RESET : GPIO_PIN_SET;
#ifdef INTERNAL_LED_RED_GPIO_Port
    HAL_GPIO_WritePin(INTERNAL_LED_RED_GPIO_Port, INTERNAL_LED_RED_Pin, pin_state);
#endif
#ifdef RGB_LED_RED_GPIO_Port
    HAL_GPIO_WritePin(RGB_LED_RED_GPIO_Port, RGB_LED_RED_Pin, pin_state);
#endif
#ifdef INT_RGB_LED_RED_GPIO_Port
    HAL_GPIO_WritePin(INT_RGB_LED_RED_GPIO_Port, INT_RGB_LED_RED_Pin, pin_state);
#endif
#ifdef EXT_RGB_LED_RED_GPIO_Port
    HAL_GPIO_WritePin(EXT_RGB_LED_RED_GPIO_Port, EXT_RGB_LED_RED_Pin, pin_state);
#endif
}

void greenLedSetState(bool led_state) {
    GPIO_PinState pin_state = (led_state == true) ? GPIO_PIN_RESET : GPIO_PIN_SET;
#ifdef INTERNAL_LED_GREEN_GPIO_Port
    HAL_GPIO_WritePin(INTERNAL_LED_GREEN_GPIO_Port, INTERNAL_LED_GREEN_Pin, pin_state);
#endif
#ifdef RGB_LED_GREEN_GPIO_Port
    HAL_GPIO_WritePin(RGB_LED_GREEN_GPIO_Port, RGB_LED_GREEN_Pin, pin_state);
#endif
#ifdef INT_RGB_LED_GREEN_GPIO_Port
    HAL_GPIO_WritePin(INT_RGB_LED_GREEN_GPIO_Port, INT_RGB_LED_GREEN_Pin, pin_state);
#endif
#ifdef EXT_RGB_LED_GREEN_GPIO_Port
    HAL_GPIO_WritePin(EXT_RGB_LED_GREEN_GPIO_Port, EXT_RGB_LED_GREEN_Pin, pin_state);
#endif
}

void blueLedSetState(bool led_state) {
    GPIO_PinState pin_state = (led_state == true) ? GPIO_PIN_RESET : GPIO_PIN_SET;
#ifdef INTERNAL_LED_BLUE_GPIO_Port
    HAL_GPIO_WritePin(INTERNAL_LED_BLUE_GPIO_Port, INTERNAL_LED_BLUE_Pin, pin_state);
#endif
#ifdef RGB_LED_BLUE_GPIO_Port
    HAL_GPIO_WritePin(RGB_LED_BLUE_GPIO_Port, RGB_LED_BLUE_Pin, pin_state);
#endif
#ifdef INT_RGB_LED_BLUE_GPIO_Port
    HAL_GPIO_WritePin(INT_RGB_LED_BLUE_GPIO_Port, INT_RGB_LED_BLUE_Pin, pin_state);
#endif
#ifdef EXT_RGB_LED_BLUE_GPIO_Port
    HAL_GPIO_WritePin(EXT_RGB_LED_BLUE_GPIO_Port, EXT_RGB_LED_BLUE_Pin, pin_state);
#endif
}

void rgbLedUpdateState(RgbLedState_t rgb_led_state, uint32_t crnt_time_ms) {
    switch (rgb_led_state) {
        case RGB_LED_SOLID_BLUE:
            redLedSetState(false);
            greenLedSetState(false);
            blueLedSetState(true);
            break;
        case RGB_LED_SOLID_GREEN:
            redLedSetState(false);
            greenLedSetState(true);
            blueLedSetState(false);
            break;
        case RGB_LED_BLINKING_BLUE:
            redLedSetState(false);
            greenLedSetState(false);
            blueLedSetState(crnt_time_ms % 1000 > 500);
            break;
        case RGB_LED_BLINKING_GREEN:
            redLedSetState(false);
            greenLedSetState(crnt_time_ms % 1000 > 500);
            blueLedSetState(false);
            break;
        case RGB_LED_BLINKING_YELLOW:
            redLedSetState(crnt_time_ms % 1000 > 500);
            greenLedSetState(crnt_time_ms % 1000 > 500);
            blueLedSetState(false);
            break;
        case RGB_LED_BLINKING_RED:
            redLedSetState(crnt_time_ms % 1000 > 500);
            greenLedSetState(false);
            blueLedSetState(false);
            break;
        case RGB_LED_BLINKING_RED_BLUE:
            redLedSetState(crnt_time_ms % 1000 > 500);
            greenLedSetState(false);
            blueLedSetState(crnt_time_ms % 1000 <= 500);
            break;
        default:
            break;
    }
}
