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
			Timer.StartMs(20000);
			Uart.SendChar('H');
			Uart.SendChar('e');
			Uart.SendChar('l');
			Uart.SendChar('l');
			Uart.SendChar('o');
			Uart.SendChar(',');
			Uart.SendChar(' ');
			Uart.SendChar('R');
			Uart.SendChar('o');
			Uart.SendChar('m');
			Uart.SendChar('a');
			Uart.SendChar('\n');
		}
		volatile uint16_t value = Adc.Do();
		Leds.LedsRotarion();
	}
}

