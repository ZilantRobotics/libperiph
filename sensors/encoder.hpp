/**
* @file encoder.hpp
* @brief Class of Encoder
*/

#ifndef __ENCODER_HPP
#define __ENCODER_HPP

#include "stm32f3xx.h"

class Encoder
{
	public:
		Encoder() {};
		void Init();
		uint32_t GetPulses();
		void ReserPulses();
	private:
		
};


#endif //__ULTRASONIC_RANGEFINDER_HPP
