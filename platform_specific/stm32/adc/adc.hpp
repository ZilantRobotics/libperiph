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

#include "stm32f3xx.h"


/**
* @brief Driver ADC1
*/
struct ADC
{
	void Init();				/// Init ADC1
	uint16_t Do();				/// Start new process and return last value
};


#endif //PLATFORM_SPECIFIC_STM32_ADC_H_
