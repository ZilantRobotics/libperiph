## Analog-to-digital converter (ADC) HAL

The usage of hal_adc driver is self documented in [hal_adc.h header file](hal_adc.h). Please refer to it for details.

Typically, you need to work with the following functions:

```c++
int8_t adcInitDma(uint8_t num_of_channels);
int8_t adcGetAll(uint16_t adc_measurements[]);
uint16_t adcGet(uint8_t rank);
```

You can check an example based on Ubuntu implementation in [the unit tests](../../tests/periphery/test_adc.cpp).
