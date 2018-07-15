#ifndef __ADC_HPP
#define __ADC_HPP

#include "stm32f3xx.h"


/**
* @brief Driver ADC
*/
struct ADC
{
	void Init();
	uint16_t Do();
};


#endif //__ADC_HPP
