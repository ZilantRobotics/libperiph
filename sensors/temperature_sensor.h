/**
* @file temperature_sensor.h
* @brief Class of TemperatureSensor
*/

#ifndef __TEMPERATURE_SENSOR_HPP
#define __TEMPERATURE_SENSOR_HPP

#include "stdint.h"

/**
* @brief Get temperature value
* @return temperature value in degree celsius
*/
uint32_t temperature_sensor_parse(int32_t raw_adc_value);

#endif //__TEMPERATURE_SENSOR_HPP
