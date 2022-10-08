/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file adc.hpp
 * @author d.ponomarev
 * @date Jun 25, 2018
 */

#ifndef PLATFORM_SPECIFIC_STM32_ADC_H_
#define PLATFORM_SPECIFIC_STM32_ADC_H_

#include <stdint.h>

/**
 * @note use when your DMA is free
 */
int8_t adcInitDma(uint8_t num_of_channels);
int8_t adcGetAll(uint16_t adc_measurements[]);
uint16_t adcGet(uint8_t rank);


/**
 * @note use when your DMA is not free
 */
int8_t adcInitWithoutDma(uint8_t num_of_channels);
int8_t adcMeasureWithoutDma(uint16_t values[]);

#endif //PLATFORM_SPECIFIC_STM32_ADC_H_
