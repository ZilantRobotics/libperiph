## Serial Peripheral Interface (SPI) HAL

The usage of hal_spi driver is self documented in [hal_spi.h header file](hal_spi.h). Please refer to it for details.

Typically, you need to work with the following functions:

```c++
int8_t spiPerformTransaction(uint8_t* tx, uint8_t* rx, uint8_t size);
```

## stm32 usage

If you want to use stm32 platform, you should specify `LIBPERIPH_PLATFORM=stm32`.

It is expected to configure the periphery in STM32CubeMX.

You should enable SPI as shown below:

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/periphery/hal_spi/spi1_mode_and_configuration.png?raw=true" alt="drawing">

## Ubuntu usage

If you want to use Ubuntu platform, you should specify `LIBPERIPH_PLATFORM=ubuntu`.

Ubuntu platform might used either for unit tests or for SITL mode.

For unit tests example based on Ubuntu implementation please check [tests/periphery/test_spi.cpp](../../tests/periphery/test_spi.cpp).

In SITL mode you should implement your own version of `int8_t ubuntuSpiCallback(const uint8_t* tx, uint8_t* rx, uint8_t size)` function:

```c++
int8_t ubuntuSpiCallback(const uint8_t* tx, uint8_t* rx, uint8_t size) {
    // fill rx buffer based on the tx input
    return LIBPERIPH_OK;
}
```

By default, this fuction has a weak attribute and do nothing.

Using this function you can emulate the real SPI in a way you want. In the unit tests there is an example of RM3100 emulation.
