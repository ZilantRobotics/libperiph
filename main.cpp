/**
* @file main.cpp
* @brief Реализация основного рабочего цикла
*/

#include "stm32f3xx.h"
#include <target.hpp>
#include <soft_timer.hpp>
#include <leds_rotation.hpp>
#include <adc.hpp>

extern TargetBase Target;
extern LedsRotation Leds;
extern ADC Adc;

int main()
{
    Target.InitGPIO();
	Adc.Init();
    while (1)
    {
		volatile uint16_t value = Adc.Do();
		Leds.LedsRotarion();
	}
}

