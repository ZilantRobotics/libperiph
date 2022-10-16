/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SRC_HAL_STM32_HAL_TIMERS_H_
#define SRC_HAL_STM32_HAL_TIMERS_H_


#include <stdint.h>


typedef enum {
    CH_A1_PIN_PB7 = 0,      // TIM4_CH2    I2C1_SDA
    CH_A2_PIN_PB6,          // TIM4_CH1    I2C1_SCL

    CH_B1_PIN_A3,           // TIM2_CH4    ADC12_IN3   USART2_RX
    CH_B2_PIN_A2,           // TIM2_CH3    ADC12_IN2   USART2_TX

    CH_C1_PIN_A9_TIM1_2,    // TIM1_CH2    USART1_TX
    CH_C2_PIN_A8_TIM1_1,    // TIM1_CH1    USART1_CK
    CH_C3_PIN_A10_TIM1_3,   // TIM1_CH3                             starter_throttle

    TIM_3_CH_1_PIN_PB4,     // TIM3_CH1                             rgb led
    TIM_3_CH_2_PIN_PB5,     // TIM3_CH2                             rgb led
    TIM_3_CH_3_PIN_PB0,     // TIM3_CH3                             rgb led

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

#endif  // SRC_HAL_STM32_HAL_TIMERS_H_
