# AS5600 Magnetic Angle Sensor Encoder Module

12-Bit Programmable Contactless Potentiometer

|   |   |
| - | - |
| Reference | [Datasheet](https://ams.com/documents/20143/36005/AS5600_DS000365_5-00.pdf/649ee61c-8f9a-20df-9e10-43173a3eb323) |
| Requred periphery | I2C |
| Unit tests | not yet |


## Usage

Installation:
1. Include the header file `as5600.h` and source file `as5600.c` in your C/C++ code.
2. Ensure that the required dependencies are installed, such as the STM32 HAL library or any other libraries specific to your STM32 development environment.

Usage:
1. During the boot of your application initialize I2C driver and then call `as5600Init()`
2. Periodically (with rate 10 Hz) call `as5600CollectData()`. If it returns `LIBPERIPH_OK`, then call `as5600ParseCollectedData()`.

## Additional info

- https://ams.com/documents/20143/36005/AS5600_DS000365_5-00.pdf
- https://github.com/nicholasmorrow/AS5600
- https://github.com/Seeed-Studio/Seeed_Arduino_AS5600
