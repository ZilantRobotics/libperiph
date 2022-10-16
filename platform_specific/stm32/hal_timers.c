/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_timers.h"
#include "main.h"
#include "config.h"


#if defined(CAN_PWM_C_CHANNELS)
    extern TIM_HandleTypeDef htim1;
#endif

#if defined(CAN_PWM_B1_CHANNEL) || defined(CAN_PWM_B2_CHANNEL)
    extern TIM_HandleTypeDef htim2;
#endif

#if defined(CAN_PWM_D1_CHANNEL) || defined(CAN_PWM_D2_CHANNEL)
    extern TIM_HandleTypeDef htim3;
#endif

#if defined(CAN_PWM_A1_CHANNEL) || defined(CAN_PWM_A2_CHANNEL)
    extern TIM_HandleTypeDef htim4;
#endif

typedef struct {
    volatile uint32_t* const ccr;
    TIM_HandleTypeDef* const htim;
    const uint32_t tim_channel;
    TimerMode_t mode;
} TimChannel_t;

static volatile TimChannel_t channels[TIM_CH_AMOUNT] = {
#if defined(CAN_PWM_A1_CHANNEL) || defined(CAN_PWM_A2_CHANNEL)
    {&TIM4->CCR2,   &htim4, TIM_CHANNEL_2,  TIMER_NOT_CONFIGURED},  // PB7  TIM4_CH2
    {&TIM4->CCR1,   &htim4, TIM_CHANNEL_1,  TIMER_NOT_CONFIGURED},  // PB6  TIM4_CH1
#else
    {NULL,          NULL,   TIM_CHANNEL_2,  TIMER_NOT_CONFIGURED},
    {NULL,          NULL,   TIM_CHANNEL_1,  TIMER_NOT_CONFIGURED},
#endif

#if defined(CAN_PWM_B1_CHANNEL) || defined(CAN_PWM_B2_CHANNEL)
    {&TIM2->CCR4,   &htim2, TIM_CHANNEL_4,  TIMER_NOT_CONFIGURED},  // PA3  TIM2_CH4
    {&TIM2->CCR3,   &htim2, TIM_CHANNEL_3,  TIMER_NOT_CONFIGURED},  // PA2  TIM2_CH3
#else
    {NULL,          NULL,   TIM_CHANNEL_4,  TIMER_NOT_CONFIGURED},
    {NULL,          NULL,   TIM_CHANNEL_3,  TIMER_NOT_CONFIGURED},
#endif

#if defined(CAN_PWM_C_CHANNELS)
    {&TIM1->CCR2,   &htim1, TIM_CHANNEL_2,  TIMER_NOT_CONFIGURED},  // PA9  TIM1_CH2
    {&TIM1->CCR1,   &htim1, TIM_CHANNEL_1,  TIMER_NOT_CONFIGURED},  // PA8  TIM1_CH1
    {&TIM1->CCR3,   &htim1, TIM_CHANNEL_3,  TIMER_NOT_CONFIGURED},  // PA10
#else
    {NULL,          NULL,   TIM_CHANNEL_2,  TIMER_NOT_CONFIGURED},
    {NULL,          NULL,   TIM_CHANNEL_1,  TIMER_NOT_CONFIGURED},
    {NULL,          NULL,   TIM_CHANNEL_3,  TIMER_NOT_CONFIGURED},
#endif

#if defined(CAN_PWM_D1_CHANNEL) || defined(CAN_PWM_D2_CHANNEL)
    {&TIM3->CCR1,   &htim3, TIM_CHANNEL_1,  TIMER_NOT_CONFIGURED},  // TIM_3_CH_1_PIN_PB4
    {&TIM3->CCR2,   &htim3, TIM_CHANNEL_2,  TIMER_NOT_CONFIGURED},  // TIM_3_CH_2_PIN_PB5
    {&TIM3->CCR3,   &htim3, TIM_CHANNEL_3,  TIMER_NOT_CONFIGURED},  // TIM_3_CH_3_PIN_PB0
#else
    {NULL,          NULL,   TIM_CHANNEL_1,  TIMER_NOT_CONFIGURED},
    {NULL,          NULL,   TIM_CHANNEL_2,  TIMER_NOT_CONFIGURED},
    {NULL,          NULL,   TIM_CHANNEL_3,  TIMER_NOT_CONFIGURED},
#endif
};


int8_t timerInit(Channel_t ch, TimerMode_t mode) {
    if (mode >= TIMER_MODES_AMOUNT ||
            ch >= TIM_CH_AMOUNT ||
            channels[ch].ccr == NULL ||
            channels[ch].htim == NULL ||
            timerGetMode(ch) != TIMER_NOT_CONFIGURED) {
        return STATUS_ERROR;
    }

    if (mode == TIMER_MODE_CAPTURE) {
        if (HAL_TIM_IC_Start_IT(channels[ch].htim, channels[ch].tim_channel) != HAL_OK ||
                HAL_TIM_Base_Start_IT(channels[ch].htim) != HAL_OK) {
            return STATUS_ERROR;
        }
    } else if (mode == TIMER_MODE_PWM) {
        if (HAL_TIM_PWM_Start(channels[ch].htim, channels[ch].tim_channel) == HAL_OK) {
            channels[ch].mode = mode;
        } else {
            return STATUS_ERROR;
        }
    }
    channels[ch].mode = mode;
    return STATUS_OK;
}

void timerSetPwmFrequency(Channel_t ch, uint16_t frequency_hz) {
    if (frequency_hz < 50 || frequency_hz > 400 ||
            ch >= TIM_CH_AMOUNT ||
            channels[ch].mode != TIMER_MODE_PWM) {
        return;
    }

    volatile uint32_t* arr_reg = &(channels[ch].htim->Instance->ARR);
    uint16_t period_us = 1000000 / frequency_hz;
    *arr_reg = period_us;
}

TimerMode_t timerGetMode(Channel_t channel) {
    return channels[channel].mode;
}

void timerSetPwmDuration(Channel_t channel, uint32_t pwm) {
    if (channel < TIM_CH_AMOUNT && channels[channel].mode == TIMER_MODE_PWM) {
        *channels[channel].ccr = pwm;
    }
}

int32_t timerGetPwmDuration(Channel_t channel) {
    int32_t pwm_duration;
    if (channel < TIM_CH_AMOUNT && channels[channel].mode == TIMER_MODE_PWM) {
        pwm_duration = (int32_t)(*(channels[channel].ccr));
    } else {
        pwm_duration = STATUS_ERROR;
    }
    return pwm_duration;
}

uint32_t timerGetCapturedValue(Channel_t ch) {
    if (ch >= TIM_CH_AMOUNT || channels[ch].mode == TIMER_MODE_CAPTURE) {
        return 0;
    }
    return HAL_TIM_ReadCapturedValue(channels[ch].htim, channels[ch].tim_channel);
}
