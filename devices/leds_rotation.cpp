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
	if (!(Counter & TimerDivider))
	{
		// 1. «ажигаем нужный светодиод:
		Target.LedOff(LedOnNumberPrevious);
		Target.LedOn(LedOnNumber);
		LedOnNumberPrevious = LedOnNumber;
		
		// 2. –егулируем направление переключени€ светодиодов:
		if (DirectionOfRotation & COUNTERCLOCKWISE_ACCELERATION)
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
			if ( ((DirectionOfRotation == CLOCKWISE_ACCELERATION) || (DirectionOfRotation == COUNTERCLOCKWISE_ACCELERATION)) )
			{
				if (TimerDivider != 16383)						
					TimerDivider = (TimerDivider << 1) + 1;
				else													// if more slow
					DirectionOfRotation = 3 & (~DirectionOfRotation);	// мен€ем направление вращени€ и ускорение хитрым способом
			}
			else												
			{
				if (TimerDivider > 2)							
					TimerDivider = TimerDivider >> 1;
				else													// if more fast
					DirectionOfRotation &= ~1;	 						// мен€ем ускорение хитрым способом
			}
		}
	}
	Counter++;
}
