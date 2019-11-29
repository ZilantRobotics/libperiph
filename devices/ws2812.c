/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ws2812.c
 * @author d.ponomarev
 * @note
 * WS2812B datasheet: https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf
 * Required config for timer:
 * - timer freq = 72 000 000
 * - timer prescaller = 0
 * - pwm period = 90 ticks = 1.25 us:
 * - 25 ticks = 0.347 us
 * - 65 ticks = 0.903 us
 * - 1 sequence = 100 frames = 0.125 ms
 */

#include <stdbool.h>
#include "ws2812.h"

#define BUF_OFFSET          4
#define BITS_PER_SHADE      8
#define PWM_PERIOD_LOW      25
#define PWM_PERIOD_HIGH     65
#define MAX_BUF_SIZE        2 * BUF_OFFSET + MAX_NUM_OF_LEDS * SHADES_PER_LED * BITS_PER_SHADE

static uint8_t LEDS_NUM = 0;
static uint8_t BUF_SIZE = 0;
static uint16_t crc_values[MAX_BUF_SIZE] = {};
static TIM_HandleTypeDef* timer = NULL; 
static uint32_t timerChannel = 0;


int8_t rgbLedsInit(uint8_t numberOfLeds, TIM_HandleTypeDef* timerPtr, uint32_t channel){
    if(numberOfLeds > 4 || timerPtr == NULL){
        LEDS_NUM = 0;
        BUF_SIZE = 0;
        return STATUS_ERROR;
    }
    LEDS_NUM = numberOfLeds;
    BUF_SIZE = 2 * BUF_OFFSET + numberOfLeds * SHADES_PER_LED * BITS_PER_SHADE;
    timer = timerPtr;
    timerChannel = channel;
    return STATUS_OK;
}

/**
  * @note hack:
  * - sometimes RED is PURPLE, it probably means that the first bit was ignored
  * - sometimes BLUS is Yellow, it probably means that the first bit was ignored too
  * - So, we increase buffer size from necessary 96 to 104, by adding 4 bytes at the start and
  * at the end of buffer filled by zeros.
  * This increases stability of leds.
  */
void rgbLedsMapColorToPwm(const Leds_Color_t* ledsColor){
    const uint8_t SHADES_AMOUNT = LEDS_NUM * SHADES_PER_LED;
    for(size_t shade_num = 0; shade_num < SHADES_AMOUNT; shade_num++){
        for(size_t bit_num = 0; bit_num < BITS_PER_SHADE; bit_num++){
            bool is_bit_high = ledsColor->shades[shade_num] >> (7 - bit_num) & 0x01;
            uint16_t crc_value = (is_bit_high) ? PWM_PERIOD_HIGH : PWM_PERIOD_LOW;
            crc_values[BUF_OFFSET + shade_num * BITS_PER_SHADE + bit_num] =  crc_value;
        }
    }
}

void rgbLedsStart(){
    if(timer != NULL){
        HAL_TIM_PWM_Start_DMA(timer, timerChannel, (uint32_t*)crc_values, BUF_SIZE);
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
    if(htim != NULL && htim == timer){
        HAL_TIM_PWM_Stop_DMA(timer, timerChannel);
    }
}
