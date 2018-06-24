/**
* @file target.cpp
* @brief –еализаци€ класса мигани€ светодиодами по кругу с ускорением и замедлением
*/

#include <target.hpp>
#include <leds_rotation.hpp>

LedsRotation Leds;
extern TargetBase Target;

/**
* @brief ћигание светодиодами по очереди с разной скоростью
*/
void LedsRotation::LedsRotarion()
{
	if (Timer.GetStatus() != TIMER_WORKING)
	{
		// 1. «ажигаем нужный светодиод:
		Target.LedOff(LedOnNumberPrevious);
		Target.LedOn(LedOnNumber);
		LedOnNumberPrevious = LedOnNumber;
		
		// 2. –егулируем направление переключени€ светодиодов:
		if (Direction & COUNTERCLOCKWISE_ACCELERATION)
		{
			if (LedOnNumber == TargetBase::LD_MIN)
				LedOnNumber = TargetBase::LD_MAX;
			else
				LedOnNumber--;
		}
		else // if clockwise
		{
			if (LedOnNumber == TargetBase::LD_MAX)
				LedOnNumber = TargetBase::LD_MIN;
			else
				LedOnNumber++;
		}
		// 3. –егулируем скорость переключени€ светодиодов:
		if (LedOnNumber == 10)
		{
			if ( ((Direction == CLOCKWISE_ACCELERATION) || (Direction == COUNTERCLOCKWISE_ACCELERATION)) )
			{
				if (BlinkPeriod != BLINK_PERIOD_MAX)						
					BlinkPeriod += BLINK_PERION_ITERATION;
				else// (more slow)
					Direction = 3 & (~Direction);	// хитра€ формула
			}
			else												
			{
				if (BlinkPeriod > BLINK_PERIOD_MIN)							
					BlinkPeriod -= BLINK_PERION_ITERATION;
				else// (more fast)
					Direction &= ~1;	// хитра€ формула
			}
		}
		Timer.StartMs(BlinkPeriod);
	}
}
