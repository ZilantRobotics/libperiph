/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ws2812.h
 * @author d.ponomarev
 */

#ifndef WS2812_H_
#define WS2812_H_

#include <stdint.h>
#include <stm32f1xx_hal.h>

#define MAX_NUM_OF_LEDS     4
#define SHADES_PER_LED      3

typedef union{
    struct{
        uint8_t green;
        uint8_t red;
        uint8_t blue;
    }shades;
    uint8_t raw[SHADES_PER_LED];
}Color_t;

typedef union{
    Color_t colors[MAX_NUM_OF_LEDS];
    uint8_t shades[MAX_NUM_OF_LEDS * SHADES_PER_LED];
}Leds_Color_t;

int8_t rgbLedsInit(uint8_t numberOfLeds, TIM_HandleTypeDef* timerPtr, uint32_t channel);
void rgbLedsMapColorToPwm(const Leds_Color_t* ledsColor);
void rgbLedsStart();

#endif  // INC_WS2812_H_