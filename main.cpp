/**
* @file target.cpp
* @brief Реализация основного рабочего цикла
*/

#include "stm32f3xx.h"
#include <target.hpp>
#include <leds_rotation.hpp>

extern TargetBase Target;
extern LedsRotation Leds;

int main()
{
    Target.InitGPIO();
    
    while (1)
    {
		Leds.LedsRotarion();
		//happy_birthday();
		//indicators(counter / 10000);
	}
}

