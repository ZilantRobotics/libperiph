/**
* @file main.cpp
* @brief Main cycle implementation
*/

#include "stm32f3xx.h"
#include <target.hpp>
#include <soft_timer.hpp>
#include <motor.hpp>
#include <leds_rotation.hpp>
#include <adc.hpp>
#include <text.hpp>
#include <seven_segments_indicators.hpp>
#include <esp8266.hpp>
#include <debug.hpp>
#include <temperature_sensor.hpp>
#include <encoder.hpp>
//#include <dma.hpp>

extern LedsRotation Leds;
extern ADC Adc;
extern DebugPort Debug;
Indicators Indicator;
static SoftTimer Timer;
static SoftTimer TimerIndicator;
static SoftTimer RealTime;
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
	RealTime.StartMs(1);
	TemperatureSensor temperature;
	Encoder* encoder = Encoder::GetInstance(Encoder::LEFT_ENCODER);
	
	Motor motor;
	motor.Init();
	
	/// Main cycle
    while (1)
    {
		if (Timer.GetStatus() != SoftTimer::WORKING)
		{
			Timer.StartMs(1000);
			Debug.Transmit(temperature.Do());
			Debug.Transmit(encoder->GetPulses());
			Debug.Transmit(RealTime.GetElapsedTime());
		}
		
		/// Leds - rotation
		Leds.Do();
	}
}

