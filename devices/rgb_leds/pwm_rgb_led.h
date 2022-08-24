/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file pwm_rgb_led.h
 * @author d.ponomarev
 * @date Nov 08, 2021
 */

#ifndef SRC_DRIVERS_INC_LED_RGB_H_
#define SRC_DRIVERS_INC_LED_RGB_H_

#include "rgb_color.h"

int8_t ledRgbInit();
void ledRgbSetColors(Color_t color);

#endif  // SRC_DRIVERS_INC_LED_RGB_H_
