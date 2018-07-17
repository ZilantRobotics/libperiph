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
//#include <dma.hpp>

extern TargetBase Target;
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
    Target.InitGPIO();
	Adc.Init();
	Debug.Init();
	Wifi.Init();
	Timer.StartMs(1000);
	
	/// Variables init
	uint8_t buffer[256] = {0};
	uint8_t length = 4;
	uint8_t indicatorType = 0;
	uint16_t timeCount = 0;
	
	/// Main cycle
    while (1)
    {
		uint16_t value;
		/// If timer has tripped: start ADC, show in Debug "kek" and show in indicators ADC value
		if (Timer.GetStatus() != TIMER_WORKING)
		{
			Timer.StartMs(10000);
			value = Adc.Do() * 0.7326;	///< 3000/2^12
			num2str(value, buffer);
			Debug.Transmit(buffer, length);
			Debug.Transmit("\n");
			Wifi.Transmit(buffer, length);
			indicatorType = !indicatorType;
		}

		/// Sevensegments indicators - show value
		if (indicatorType != 0)
			Indicator.SetNumber( (uint16_t)(value*0.1) );	/// show voltage
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

