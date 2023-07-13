## HAL drivers

Some drivers do not require any HAL specific functions, but other expected to have an outside implementation of a few functions.

## Periphery

The supported peripherals are listed in the table below:

|||
|-|-|
|[hal_adc](hal_adc/README.md) | Analog-to-digital converter
|[hal_i2c](hal_i2c/README.md) | Inter-Integrated Circuit
|[hal_spi](hal_spi/README.md) | Serial Peripheral Interface
|[hal_timers](hal_timers/README.md) | Hardware timers (PWM or input capture mode)
|[hal_uart](hal_uart/README.md) | Universal asynchronous receiver / transmitter

Please, refer to the specific periphery for the details.

## Supported platforms

Here are the few examples of the implementation:
1. An example for stm32f103 based on STM32Cube Hardware Abstraction Layer (HAL) library.
2. An example of dummy implementation for Ubuntu to be able to run a libperiph based application in Software In The Loop (SITL) mode.

## Usage

Normally, you don't need to directly work with these drivers. They are for internal purposes only.

Otherwise, if you want to use them manually, it is recommended to define `LIBPERIPH_PLATFORM` to either `stm32` or `ubuntu` and then include a related CMakeLists.txt. After that, you will get `LIBPERIPH_PERIPHERY_SOURCE` and `LIBPERIPH_PERIPHERY_HEADERS` CMake variables with required source and header files.
