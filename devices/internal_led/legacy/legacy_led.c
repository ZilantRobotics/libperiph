/*
 * Copyright (C) 2019-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file legacy_led.c
 * @author d.ponomarev
 * @date Dec 07, 2022
 */

#include "legacy_led.h"
#include "main.h"

static bool rgb[3];

void ledPulseSequence(uint8_t num_of_pulses) {
    const uint32_t SEQUENCE_PERIOD = 4000;
    const uint32_t PULSES_PER_SEQUENCE = 10;
    const uint32_t PULSE_PERIOD = SEQUENCE_PERIOD / PULSES_PER_SEQUENCE;
    const uint32_t PULSE_HALF_PERIOD = PULSE_PERIOD / 2;

    if (num_of_pulses == 1) {
        rgb[0] = false;
        rgb[1] = true;
        rgb[2] = false;
    } else if (num_of_pulses == 2) {
        rgb[0] = false;
        rgb[1] = false;
        rgb[2] = true;
    } else if (num_of_pulses < 4) {
        rgb[0] = true;
        rgb[1] = true;
        rgb[2] = false;
    } else {
        rgb[0] = true;
        rgb[1] = false;
        rgb[2] = false;
    }

    uint32_t time_now = HAL_GetTick();
    uint8_t pulse_index = (time_now % SEQUENCE_PERIOD) / PULSE_PERIOD;
    if ((time_now % PULSE_PERIOD) > PULSE_HALF_PERIOD) {
        ledSetState(false);
    } else if (num_of_pulses >= pulse_index + 1) {
        ledSetState(true);
    } else {
        ledSetState(false);
    }
}

void ledToggle() {
    ///< Legacy led
#ifdef LED_GPIO_Port
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
#endif

    ///< Main internal led
#ifdef INTERNAL_LED_1_GPIO_Port
    HAL_GPIO_TogglePin(INTERNAL_LED_1_GPIO_Port, INTERNAL_LED_1_Pin);
#endif

    ///< Alternative internal led
#ifdef INTERNAL_LED_2_GPIO_Port
    HAL_GPIO_TogglePin(INTERNAL_LED_2_GPIO_Port, INTERNAL_LED_2_Pin);
#endif
}

void ledSetState(bool state) {
    GPIO_PinState pin_state = state ? GPIO_PIN_SET : GPIO_PIN_RESET;
    ///< Legacy led
#ifdef LED_GPIO_Port
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, pin_state);
#endif

    ///< Main internal led
#ifdef INTERNAL_LED_1_GPIO_Port
    HAL_GPIO_WritePin(INTERNAL_LED_1_GPIO_Port, INTERNAL_LED_1_Pin, pin_state);
#endif

    ///< Alternative internal led
#ifdef INTERNAL_LED_2_GPIO_Port
    HAL_GPIO_WritePin(INTERNAL_LED_2_GPIO_Port, INTERNAL_LED_2_Pin, pin_state);
#endif
}
