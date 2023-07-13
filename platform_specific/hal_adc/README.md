## Analog-to-digital converter (ADC) HAL

The usage of hal_adc driver is self documented in [hal_adc.h header file](hal_adc.h). Please refer to it for details.

Typically, you need to work with the following functions:

```c++
int8_t adcInitDma(uint8_t num_of_channels);
int8_t adcGetAll(uint16_t adc_measurements[]);
uint16_t adcGet(uint8_t rank);
```

## stm32 usage

If you want to use stm32 platform, you should specify `LIBPERIPH_PLATFORM=stm32`.

It is expected to configure the periphery in STM32CubeMX.

You should enable `scan conversion mode` as shown below:

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/periphery/hal_adc/parameter_settings.png?raw=true" alt="drawing">

If you are going to use DMA, don't forget to select `Peripheral to Memory` with width `Half word` as shown below:

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/periphery/hal_adc/dma_settings.png?raw=true" alt="drawing">


## Ubuntu usage

If you want to use Ubuntu platform, you should specify `LIBPERIPH_PLATFORM=ubuntu`.

Ubuntu platform might used either for unit tests or for SITL mode.

For unit tests example based on Ubuntu implementation please check [tests/periphery/test_adc.cpp](../../tests/periphery/test_adc.cpp).

In SITL mode you should implement your own version of ubuntu_adc array:

```c++
uint16_t ubuntu_adc[NUMBER_OF_ADC_CHANNELS];
```

By default, this array has a weak attribute.

So, you can change the values of ADC in real time to emulate the real ADC in a way you want.
