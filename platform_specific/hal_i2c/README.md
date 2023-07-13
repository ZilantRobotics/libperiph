## Inter-Integrated Circuit (I2C) HAL

The usage of hal_i2c driver is self documented in [hal_i2c.h header file](hal_i2c.h). Please refer to it for details.

Typically, you need to work with the following functions:

```c++
int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len);
int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len);
```

You can check an example based on Ubuntu implementation in [the unit tests](../../tests/periphery/test_i2c.cpp).
