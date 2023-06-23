# STM32 temperature sensor

STM32 temperature sensor expected to have ADC.

## Usage example

```c++
uint16_t raw_temperature_adc; // get raw adc value
uint16_t temperature_degree = stm32TemperatureParse(raw_temperature_adc);
```

## Tests

[tests/sensors/test_stm32_temperature.cpp](../../tests/test_stm32_temperature.cpp)
