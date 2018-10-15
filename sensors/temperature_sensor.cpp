/**
* @file temperature_sensor.cpp
* @brief Implementation of TemperatureSensor
*/

#include <temperature_sensor.hpp>
#include <adc.hpp>

extern ADC Adc;

/**
* @brief Get temperature value
* @return temperature value in degree celsius
*/
uint32_t TemperatureSensor::Do()
{
	const uint8_t temp_25 = 25;
	const float voltage_at_25 = 1.43;
	const float slope = 232.558;
	const float adc_koef = 0.00073242187;
	float voltage_sense = Adc.Do()*adc_koef;
	Value = (voltage_at_25 - voltage_sense)*slope + temp_25;
	return Value;
}
