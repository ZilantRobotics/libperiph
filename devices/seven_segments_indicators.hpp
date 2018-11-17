#ifndef __SEVEN_SEGMENTS_INDICATORS_HPP
#define __SEVEN_SEGMENTS_INDICATORS_HPP

#include "stdint.h"

/**
* @brief Indicators
*/
class Indicators
{
	public:
		Indicators(): digit(0) {};
		void SetNumber(uint16_t number);
		void SetNumber(float number);
	private:
		uint8_t digit;
};


#endif	// __SEVEN_SEGMENTS_INDICATORS_HPP
