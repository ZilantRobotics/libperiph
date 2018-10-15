/**
* @file main.cpp
* @brief Main cycle implementation
*/

#include "stm32f3xx.h"
#include <target.hpp>
#include <soft_timer.hpp>
#include <leds_rotation.hpp>
#include <adc.hpp>
#include <text.hpp>
#include <seven_segments_indicators.hpp>
#include <esp8266.hpp>
#include <debug.hpp>
#include <temperature_sensor.hpp>
//#include <dma.hpp>

extern LedsRotation Leds;
extern ADC Adc;
extern DebugPort Debug;
Indicators Indicator;
static SoftTimer Timer;
static SoftTimer TimerIndicator;
extern WifiEsp8266 Wifi; 
//DMA Dma;

int main()
{
	/// Modules init
    Target::InitGPIO();
	Adc.Init();
	Debug.Init();
	//Wifi.Init();
	Timer.StartMs(1000);
	TemperatureSensor temperature;
	
	/// Variables init
	uint8_t buffer[256] = {0};
	uint8_t length = 4;
	uint8_t indicatorType = 0;
	uint16_t timeCount = 0;
	uint16_t temperatureValue = 0;
	
	/// Main cycle
    while (1)
    {
		//uint16_t voltage;
		/// If timer has tripped: start ADC, show in Debug "kek" and show in indicators ADC value
		if (Timer.GetStatus() != TIMER_WORKING)
		{
			Timer.StartMs(1000);
			//volatile uint16_t val = Adc.Do();
			//voltage = Adc.Do() * 0.7326;	///< 3000/2^12
			
			temperatureValue = temperature.Do();
			
			Debug.Transmit(temperatureValue);
			//Wifi.Transmit(buffer, length);
			// TEMPERATURE
			
			
			
			//indicatorType = !indicatorType;
		}

		/// Sevensegments indicators - show value
		if (indicatorType == 0)
			//Indicator.SetNumber( (uint16_t)(voltage*0.1) );	/// show voltage
			Indicator.SetNumber(temperatureValue);	// show temperature
		else
			Indicator.SetNumber(timeCount);	/// show time (in second)
		
		/// Sevensegments indicators - counter
		if (TimerIndicator.GetStatus() != TIMER_WORKING)
		{
			TimerIndicator.StartMs(1000);
			timeCount++;
		}
		
		/// Leds - rotation
		Leds.Do();
	}
}

