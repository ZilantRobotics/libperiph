/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file adc.cpp
 * @author d.ponomarev
 * @date Jun 25, 2018
 * @note stm32f103:
 * There are 2 adc (ADC1, ADC2) with 16 channels both.
 * There are 2 types:
 * 1. Injected (up to 4) - have separate cell for each channel
 * 2. Regular (no limit) - only one cell, but you can use DMA
 */

#include "hal_adc.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "config.h"


#define MAX_CHANNELS_NUM    8

#ifdef HAL_ADC_MODULE_ENABLED
extern ADC_HandleTypeDef hadc1;
#define ADC_PTR &hadc1
#endif


static uint8_t number_of_channels = 0;
static bool is_adc_already_inited = false;
static uint16_t adc_raw[MAX_CHANNELS_NUM];


int8_t adcInitDma(uint8_t num_of_channels) {
#ifdef ADC_PTR
    if (is_adc_already_inited == true) {
        return STATUS_ERROR;
    }
    if (num_of_channels > MAX_CHANNELS_NUM) {
        return STATUS_ERROR;
    }
    if (HAL_ADCEx_Calibration_Start(ADC_PTR) != HAL_OK) {
        return STATUS_ERROR;
    }
    if (HAL_ADC_Start_DMA(ADC_PTR, (uint32_t*)&adc_raw, num_of_channels) != HAL_OK) {
        return STATUS_ERROR;
    }
    is_adc_already_inited = true;
    number_of_channels = num_of_channels;
    return STATUS_OK;
#endif
    return STATUS_ERROR;
}

int8_t adcGetAll(uint16_t* adc_measurements) {
    if (is_adc_already_inited) {
        memcpy((void*)adc_measurements, (void*)adc_raw, number_of_channels * sizeof(uint16_t));
        return STATUS_OK;
    }
    return STATUS_ERROR;
}

uint16_t adcGet(uint8_t rank) {
    if (is_adc_already_inited && rank < MAX_CHANNELS_NUM) {
        return adc_raw[rank];
    }
    return 0;
}


int8_t adcInitWithoutDma(uint8_t num_of_channels) {
#ifdef HAL_ADC_MODULE_ENABLED
    if (is_adc_already_inited || num_of_channels == 0 || num_of_channels > MAX_CHANNELS_NUM) {
        return STATUS_ERROR;
    }
    if (HAL_ADCEx_Calibration_Start(ADC_PTR) == HAL_OK) {
        is_adc_already_inited = true;
        number_of_channels = num_of_channels;
        return STATUS_OK;
    }
#endif
    return STATUS_ERROR;
}

int8_t adcMeasureWithoutDma(uint16_t values[]) {
    if (!is_adc_already_inited) {
        return STATUS_ERROR;
    }

    #ifdef HAL_ADC_MODULE_ENABLED
    {
        HAL_ADC_Start(ADC_PTR);
        for (size_t ch_idx = 0; ch_idx < number_of_channels; ch_idx++) {
            values[ch_idx] = (uint16_t)HAL_ADC_GetValue(ADC_PTR);
        }
        return STATUS_OK;
    }
    #endif

    return STATUS_ERROR;
}

#ifdef HAL_ADC_MODULE_ENABLED
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {
        HAL_ADC_Start_DMA(ADC_PTR, (uint32_t*)&adc_raw, number_of_channels);
    }
}
#endif
