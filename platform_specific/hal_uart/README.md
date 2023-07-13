## Universal asynchronous receiver / transmitter (UART) HAL

The usage of hal_uart driver is self documented in [hal_uart.h header file](hal_uart.h). Please refer to it for details.

Typically, you need to work with the following functions:

```c++
int8_t uartInitRx(UartInstance_t instance, UartType_t, uint8_t buffer[], uint16_t size);
size_t uartGetLastReceivedIndex(UartInstance_t instance);

int8_t uartTransmit(UartInstance_t instance, uint8_t buffer[], size_t size);
```

## stm32 usage

If you want to use stm32 platform, you should specify `LIBPERIPH_PLATFORM=stm32`.

It is expected to configure the periphery in STM32CubeMX.

You should enable UART as shown below:

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/periphery/hal_uart/usart1_mode_and_configuration.png?raw=true" alt="drawing">

## Ubuntu usage

If you want to use Ubuntu platform, you should specify `LIBPERIPH_PLATFORM=ubuntu`.

Ubuntu platform might used either for unit tests or for SITL mode.

For unit tests example based on Ubuntu implementation please check [tests/periphery/test_uart.cpp](../../tests/periphery/test_uart.cpp).

In SITL mode you should implement your own version of ubuntu_uart arrays:

```c++
uint8_t ubuntu_uart_last_recv[2] = {};
uint8_t* ubuntu_uart_rx_buffer[2] = {};

uint8_t ubuntu_uart_tx_buffer[256] = {};
```

By default, this array has a weak attribute.

So, you can change the values of UART in real time to emulate the real UART in a way you want.
