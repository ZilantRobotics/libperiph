/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef PLATFORM_SPECIFIC_HAL_TIMERS_H_
#define PLATFORM_SPECIFIC_HAL_TIMERS_H_

#include <stdint.h>

typedef enum {
    PIN_PB7_TIM4_CH2 = 0,   // I2C1_SDA
    PIN_PB6_TIM4_CH1,       // I2C1_SCL

    PIN_A3_TIM2_CH4,        // ADC12_IN3    USART2_RX
    PIN_A2_TIM2_CH3,        // ADC12_IN2    USART2_TX

    PIN_A9_TIM1_CH2,        // USART1_TX
    PIN_A8_TIM1_CH1,        // USART1_CK
    PIN_A10_TIM1_CH3,       //                          starter_throttle

    PIN_PB4_TIM3_CH1,       //                          rgb led
    PIN_PB5_TIM3_CH2,       //                          rgb led
    PIN_PB0_TIM3_CH3,       //                          rgb led

    // TIM4_CH3
    // TIM4_CH4

    TIM_CH_AMOUNT,
} Channel_t;

typedef enum {
    TIMER_MODE_CAPTURE = 0,
    TIMER_MODE_PWM,
    TIMER_MODES_AMOUNT,

    TIMER_NOT_CONFIGURED = 0xFF
} TimerMode_t;


/**
  * @brief Perform channel initialization into desired mode
  * @return STATUS_ERROR if channel already has been initialized, else return STATUS_OK
  * @note It is highly reccomended to set default pwm as soon as possible after initialization!
  */
int8_t timerInit(Channel_t channel, TimerMode_t mode);
TimerMode_t timerGetMode(Channel_t channel);


void timerSetPwmFrequency(Channel_t ch, uint16_t frequency_hz);

/**
  * @brief PWM features
  */
void timerSetPwmDuration(Channel_t channel, uint32_t pwm);
int32_t timerGetPwmDuration(Channel_t channel);


/**
  * @brief Input capture features
  */
uint32_t timerGetCapturedValue(Channel_t channel);

#endif  // PLATFORM_SPECIFIC_HAL_TIMERS_H_
