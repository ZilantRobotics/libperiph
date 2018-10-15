/**
* @file temperature_sensor.hpp
* @brief Class of TemperatureSensor
*/

#ifndef __TEMPERATURE_SENSOR_HPP
#define __TEMPERATURE_SENSOR_HPP

#include "stm32f3xx.h"

class TemperatureSensor
{
	public:
		TemperatureSensor() {};
		uint32_t Do();
	private:
		uint32_t Value;

};


#endif //__TEMPERATURE_SENSOR_HPP
