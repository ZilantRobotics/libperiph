## Universal asynchronous receiver / transmitter (UART) HAL

The usage of hal_uart driver is self documented in [hal_uart.h header file](hal_uart.h). Please refer to it for details.

Typically, you need to work with the following functions:

```c++
int8_t uartInitRx(UartInstance_t instance, UartType_t, uint8_t buffer[], uint16_t size);
size_t uartGetLastReceivedIndex(UartInstance_t instance);
uint8_t* uartRxDmaPop(UartInstance_t instance);

int8_t uartTransmit(UartInstance_t instance, uint8_t buffer[], size_t size);
```

You can check an example based on Ubuntu implementation in [the unit tests](../../tests/periphery/test_uart.cpp).
