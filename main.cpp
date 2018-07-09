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
extern UART UART1;
extern UART UART2;
UART* Debug = &UART1;
UART* ESP8266 = &UART2;

DMA Dma;

int main()
{
	SoftTimer Timer;
	
    Target.InitGPIO();
	Adc.Init();
	Debug->Init(UART_1);
	ESP8266->Init(UART_2);
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
			Debug->SendArr(buf, 6);
			Debug->SendArr("mV\n", 3);
			ESP8266->SendArr(buf, 6);
			ESP8266->SendArr("mV\n", 3);
			
			// UART receive
			Debug->GetData(ptrUartRX, length);
			ptrUartRX[0]++;
			Debug->SendArr(ptrUartRX, length);
		}
		
		Leds.LedsRotarion();
	}
}

