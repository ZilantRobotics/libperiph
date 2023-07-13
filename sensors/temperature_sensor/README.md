# STM32 temperature sensor

STM32 temperature sensor expected to have ADC.

## Usage example

```c++
uint16_t raw_temperature_adc; // get raw adc value
uint16_t temperature_degree = stm32TemperatureParse(raw_temperature_adc);
```

## Tests

[tests/sensors/temperature_sensor/test.cpp](../../tests/sensors/temperature_sensor/test.cpp)
