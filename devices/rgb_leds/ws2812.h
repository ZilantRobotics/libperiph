/*
 * Copyright (C) 2019-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef DEVICES_RGB_LEDS_WS2812_H_
#define DEVICES_RGB_LEDS_WS2812_H_

#include "main.h"
#include "rgb_color.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WS2812_OK        0
#define WS2812_ERROR    -1

/**
 * @brief Initialize module
 * @param number_of_leds - no more than MAX_NUM_OF_LEDS
 * @param timer_ptr - any timer
 * @param channel - any channel corresponded the timer
 * @return WS2812_ERROR if periphery or params is not ok, otherwise return WS2812_OK
 */
int8_t ws2812bInit(uint8_t number_of_leds, TIM_HandleTypeDef* timer_ptr, uint32_t channel);

/**
 * @brief   Fill DMA buffer by values corresponded desired colors
 * @note    Do not actually start PWM.
 *          This method may has an affect on resulting RGB leds color if it is called during
 *          DMA work.
 */
void ws2812bSetColors(const Leds_Color_t* leds_color);

/**
 * @brief   Run single cycle of setting saved RGB color
 * @return  WS2812_ERROR if error occured, otherwise WS2812_OK
 */
int8_t ws2812bStartOnce();

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_RGB_LEDS_WS2812_H_
