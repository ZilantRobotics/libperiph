/*
 * Copyright (C) 2019-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ws2812.h"
#include <stdbool.h>
#include <stddef.h>


#define BUF_OFFSET          4
#define BITS_PER_SHADE      8
#define PWM_PERIOD_LOW      29
#define PWM_PERIOD_HIGH     58
#define MAX_BUF_SIZE        2 * BUF_OFFSET + MAX_NUM_OF_LEDS * SHADES_PER_LED * BITS_PER_SHADE


static TIM_HandleTypeDef* timer = NULL;
static uint32_t timer_channel = 0;
static uint16_t ccr_values[MAX_BUF_SIZE] = {0x00};
static uint16_t BUF_SIZE = 0;
static uint8_t LEDS_NUM = 0;


int8_t ws2812bInit(uint8_t number_of_leds, TIM_HandleTypeDef* timer_ptr, uint32_t channel) {
    if (number_of_leds > MAX_NUM_OF_LEDS || timer_ptr == NULL) {
        LEDS_NUM = 0;
        BUF_SIZE = 0;
        return WS2812_ERROR;
    }

    LEDS_NUM = number_of_leds;
    BUF_SIZE = 2 * BUF_OFFSET + number_of_leds * SHADES_PER_LED * BITS_PER_SHADE;
    timer = timer_ptr;
    timer_channel = channel;
    return WS2812_OK;
}

/**
  * @note hack:
  * - sometimes RED is PURPLE, it probably means that the first bit was ignored
  * - sometimes BLUS is Yellow, it probably means that the first bit was ignored too
  * - So, we increase buffer size from necessary 96 to 104, by adding 4 bytes at the start and
  * at the end of buffer filled by zeros.
  * This increases stability of leds.
  */
void ws2812bSetColors(const Leds_Color_t* ledsColor) {
    const size_t SHADES_AMOUNT = LEDS_NUM * SHADES_PER_LED;
    for (size_t shade_num = 0; shade_num < SHADES_AMOUNT; shade_num++) {
        for (size_t bit_num = 0; bit_num < BITS_PER_SHADE; bit_num++) {
            bool is_bit_high = ledsColor->shades[shade_num] >> (7 - bit_num) & 0x01;
            uint16_t ccr_value = is_bit_high ? PWM_PERIOD_HIGH : PWM_PERIOD_LOW;
            ccr_values[BUF_OFFSET + shade_num * BITS_PER_SHADE + bit_num] =  ccr_value;
        }
    }
}

int8_t ws2812bStartOnce() {
    if (LEDS_NUM == 0) {
        return WS2812_ERROR;
    }
    if (HAL_TIM_PWM_Start_DMA(timer, timer_channel, (uint32_t*)ccr_values, BUF_SIZE) != HAL_OK) {
        return WS2812_ERROR;
    }
    return WS2812_OK;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim) {
    if (htim != NULL && htim == timer) {
        HAL_TIM_PWM_Stop_DMA(timer, timer_channel);
    }
}
