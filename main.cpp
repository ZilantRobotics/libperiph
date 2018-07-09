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
#include <dma.hpp>

extern TargetBase Target;
extern LedsRotation Leds;
extern ADC Adc;

DMA Dma;

int main()
{
	SoftTimer Timer;
	UART Debug;
	
    Target.InitGPIO();
	Adc.Init();
	Debug.Init();
	Timer.StartMs(1000);
	
	// Check DMA
	//Dma.Init();
	//Dma.ConfigureUSART1_TX();
	
	uint8_t ptrUartRX[256] = {0};
	uint8_t length = 0;
    while (1)
    {
		if (Timer.GetStatus() != TIMER_WORKING)
		{
			// Timer
			Timer.StartMs(500);
			
			// Adc
			uint16_t value = Adc.Do();
			value = value*0.7326;	// 3000/2^12
			uint8_t buf[6] = {0};
			num2str(value, (char*)buf);
			Debug.SendArr(buf, 6);
			Debug.SendArr("mV\n", 3);
			
			// UART receive
			Debug.GetData(ptrUartRX, length);
			Debug.SendArr(ptrUartRX, length);
		}
		
		Leds.LedsRotarion();
	}
}

