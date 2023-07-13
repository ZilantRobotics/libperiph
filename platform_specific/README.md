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

**Why do I nedd Ubuntu platform?**

The most obvious way is to use Ubuntu driver as part of unit tests. Each driver is covered with unit tests, you can check them as an example.

The simplest use case is to run a Cyphal/DroneCAN mode in SITL mode on Ubuntu and track the changes of the timer to verify that the node has the expected behaviour. You may also make it as part of ypur CI process.

The more complicated use case is to build a special SITL application for your Cyphal/DroneCAN node and itgerate it into a SITL/HITL simulation of a vehicle to comprehensively analyze the behavior of the control system.

## Usage

Normally, you don't need to directly work with these drivers. They are for internal purposes only.

Otherwise, if you want to use them manually, it is recommended to define `LIBPERIPH_PLATFORM` to either `stm32` or `ubuntu` and then include a related CMakeLists.txt. After that, you will get `LIBPERIPH_PERIPHERY_SOURCE` and `LIBPERIPH_PERIPHERY_HEADERS` CMake variables with required source and header files.

For Ubuntu platform you may write your own implementation of a few weak variables and functions to customize the defult behaviour of the driver.
