## Serial Peripheral Interface (SPI) HAL

The usage of hal_spi driver is self documented in [hal_spi.h header file](hal_spi.h). Please refer to it for details.

Typically, you need to work with the following functions:

```c++
int8_t spiPerformTransaction(uint8_t* tx, uint8_t* rx, uint8_t size);
```

You can check an example based on Ubuntu implementation in [the unit tests](../../tests/periphery/test_spi.cpp).
