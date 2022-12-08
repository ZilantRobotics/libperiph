/*
 * Copyright (C) 2019-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file legacy_led.h
 * @author d.ponomarev
 * @date Dec 07, 2022
 */

#ifndef SRC_DRIVERS_INTERNAL_LED_LEGACY_LEGACY_LED_H_
#define SRC_DRIVERS_INTERNAL_LED_LEGACY_LEGACY_LED_H_

#include <stdint.h>
#include <stdbool.h>

void ledPulseSequence(uint8_t num_of_pulses);

void ledToggle();

void ledSetState(bool state);

#endif  // SRC_DRIVERS_INTERNAL_LED_LEGACY_LEGACY_LED_H_
