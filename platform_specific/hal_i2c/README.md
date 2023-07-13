## Inter-Integrated Circuit (I2C) HAL

The usage of hal_i2c driver is self documented in [hal_i2c.h header file](hal_i2c.h). Please refer to it for details.

Typically, you need to work with the following functions:

```c++
int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len);
int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len);
```

## stm32 usage

If you want to use stm32 platform, you should specify `LIBPERIPH_PLATFORM=stm32`.

It is expected to configure the periphery in STM32CubeMX.

You should enable i2c as shown below:

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/periphery/hal_i2c/i2c1_mode_and_configuration.png?raw=true" alt="drawing">

## Ubuntu usage

If you want to use Ubuntu platform, you should specify `LIBPERIPH_PLATFORM=ubuntu`.

Ubuntu platform might used either for unit tests or for SITL mode.

For unit tests example based on Ubuntu implementation please check [tests/periphery/test_i2c.cpp](../../tests/periphery/test_i2c.cpp).

In SITL mode you should implement your own version of ubuntu_i2c_id and ubuntu_i2c_buffer array:

```c++
uint8_t ubuntu_i2c_id = 42;
uint8_t ubuntu_i2c_buffer[256] = {};
```

By default, these variables have a weak attributes.

So, you can change the values of I2C in real time to emulate the real I2C in a way you want.

