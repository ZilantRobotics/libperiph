/**
* @file main.cpp
* @brief Реализация основного рабочего цикла
*/

#include "stm32f3xx.h"
#include <target.hpp>
#include <soft_timer.hpp>
#include <leds_rotation.hpp>
#include <adc.hpp>
#include <uart.hpp>
#include <text.hpp>

extern TargetBase Target;
extern LedsRotation Leds;
extern ADC Adc;


int main()
{
	SoftTimer Timer;
	UART Uart;
	
    Target.InitGPIO();
	Adc.Init();
	Uart.Init();
	Timer.StartMs(1000);
    while (1)
    {
		if (Timer.GetStatus() != TIMER_WORKING)
		{
			Timer.StartMs(500);
			uint16_t value = Adc.Do();
			uint8_t buf[12];
			num2str(value, (char*)buf);
			Uart.SendArr(buf, 12);
			Uart.SendArr("\n", 1);
		}
		
		Leds.LedsRotarion();
	}
}

