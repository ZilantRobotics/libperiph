#ifndef __ADC_HPP
#define __ADC_HPP

#include "stm32f3xx.h"


/**
* @brief Driver ADC1
*/
struct ADC
{
	void Init();				/// Init ADC1
	uint16_t Do();				/// Start new process and return last value
};


#endif //__ADC_HPP
