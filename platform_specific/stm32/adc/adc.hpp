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
