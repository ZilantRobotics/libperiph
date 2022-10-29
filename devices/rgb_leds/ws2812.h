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

#ifndef DEVICES_RGB_LEDS_WS2812_H_
#define DEVICES_RGB_LEDS_WS2812_H_

#include "main.h"
#include "rgb_color.h"


/**
 * @brief Initialize module
 * @param number_of_leds - no more than MAX_NUM_OF_LEDS
 * @param timer_ptr - any timer
 * @param channel - any channel corresponded the timer
 * @return STATUS_ERROR if periphery or params is not ok, otherwise return STATUS_OK
 */
int8_t ws2812bInit(uint8_t number_of_leds, TIM_HandleTypeDef* timer_ptr, uint32_t channel);

/**
 * @brief   Fill DMA buffer by values corresponded desired colors
 * @note    Do not actually start PWM.
 *          This method may has an affect on resulting RGB leds color if it is called during
 *          DMA work.
 * @todo    May add condition if dma is in process now, return an error
 */
void ws2812bSetColors(const Leds_Color_t* leds_color);

/**
 * @brief   Run single cycle of setting saved RGB color
 * @return STATUS_ERROR if error occured, otherwise STATUS_OK
 */
int8_t ws2812bStartOnce();

#endif  // DEVICES_RGB_LEDS_WS2812_H_
