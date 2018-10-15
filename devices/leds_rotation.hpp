#ifndef __LEDS_ROTATION_HPP
#define __LEDS_ROTATION_HPP

#include "stm32f3xx.h"
#include "soft_timer.hpp"

/**
* @brief Class of LED's blinking
*/
class LedsRotation
{
	private:
		enum 
		{
			CLOCKWISE_ACCELERATION = 0, 
			CLOCKWISE_DECELERATION = 1, 
			COUNTERCLOCKWISE_ACCELERATION = 2, 
			COUNTERCLOCKWISE_DECELERATION = 3,
		};
		enum
		{
			BLINK_PERIOD_MIN = 10,		///< 10 ms
			BLINK_PERIOD_MAX = 100,		///< 100 ms
			BLINK_PERION_ITERATION = 10,///< 10 ms
		};
		SoftTimer Timer;
		uint8_t LedOnNumber;
		uint8_t LedOnNumberPrevious;
		uint8_t BlinkPeriod;
		uint8_t Direction;
	public:
		LedsRotation(): 
			LedOnNumber(Target::LD_MIN),
			LedOnNumberPrevious(Target::LD_MIN),
			BlinkPeriod(BLINK_PERIOD_MIN),
			Direction(CLOCKWISE_ACCELERATION) {}
		void Do();
		
};


#endif	// __LEDS_ROTATION_HPP
