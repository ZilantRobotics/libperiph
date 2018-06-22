#ifndef __LEDS_ROTATION_HPP
#define __LEDS_ROTATION_HPP

#include "stm32f3xx.h"

/**
* @brief ћигание светодиодами по кругу с разной скоростью
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
		uint32_t Counter;
		uint8_t LedOnNumber;
		uint8_t LedOnNumberPrevious;
		uint16_t TimerDivider;
		uint8_t DirectionOfRotation;
	public:
		LedsRotation(): 
			Counter(0), 
			LedOnNumber(TargetBase::LD_MIN),
			LedOnNumberPrevious(TargetBase::LD_MIN),
			TimerDivider(1),
			DirectionOfRotation(CLOCKWISE_ACCELERATION) {}
		void LedsRotarion();
		
};


#endif	// __LEDS_ROTATION_HPP
