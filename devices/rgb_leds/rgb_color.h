/*
 * Copyright (C) 2019-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef DEVICES_RGB_LEDS_RGB_COLOR_
#define DEVICES_RGB_LEDS_RGB_COLOR_

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define MAX_NUM_OF_LEDS     32
#define SHADES_PER_LED      3

#define RGB_LED_GREEN_COLOR (0xFF << 0) | (0x00 << 8) | (0x00 << 16)
#define RGB_LED_RED_COLOR   (0x00 << 0) | (0xFF << 8) | (0x00 << 16)
#define RGB_LED_BLUE_COLOR  (0x00 << 0) | (0x00 << 8) | (0xFF << 16)


typedef union {
    struct {
        uint8_t green;
        uint8_t red;
        uint8_t blue;
    } shades;
    uint8_t raw[SHADES_PER_LED];
} Color_t;

typedef union {
    Color_t colors[MAX_NUM_OF_LEDS];
    uint8_t shades[MAX_NUM_OF_LEDS * SHADES_PER_LED];
} Leds_Color_t;


inline void rgbLedClear(Leds_Color_t* leds, size_t number_of_leds) {
    if (leds == NULL || number_of_leds > MAX_NUM_OF_LEDS) {
        return;
    }

    memset(leds, 0x00, number_of_leds * 3);
}

inline void rgbLedSetValue(Color_t* color, uint32_t value) {
    if (color == NULL) {
        return;
    }

    memcpy(color, &value, 3);
}

#endif  // DEVICES_RGB_LEDS_RGB_COLOR_
