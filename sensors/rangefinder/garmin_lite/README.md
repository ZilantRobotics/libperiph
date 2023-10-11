# Garmin lite

The Garmin / PulsedLight LIDAR-Lite rangefinder is a low-cost optical distance measurement solution with a 40m range under most operating conditions, low power consumption, and small form factor.

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/garmin_lite_v3.jpg?raw=true" alt="drawing" width="256">

|   |   |
| - | - |
| Reference | [Datasheet](https://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf) |
| Requred periphery | I2C |
| Unit tests | [test_garmin_lite.cpp](../../../tests/sensors/rangefinder/test_garmin_lite.cpp) |

## Usage

Installation:
1. Include the header file `garmin_lite.h` and source file `garmin_lite.c` in your C/C++ code.
2. Ensure that the required dependencies are installed, such as the STM32 HAL library or any other libraries specific to your STM32 development environment.

Usage:
1. During the boot of your application initialize I2C driver and then call `garminLiteInit()`
2. Periodically (with rate 10 Hz) call `garminLiteCollectData()`. If it returns `LIBPERIPH_OK`, then call `garminLiteParseCollectedData()`.

## Implementation details

The simplest method of obtaining measurement results from the I2C interface is as follows:
1. Write 0x04 to register 0x00
2. Read register 0x01. Repeat until bit 0 (LSB) goes low
3. Read two bytes from 0x8f (0x0f and 0x10) to obtain 16-bit measured distance in centimeters.

Slightly optimized algorithm:
1. Read two bytes from 0x8f (0x0f and 0x10) to obtain 16-bit measured distance in centimeters
2. Write 0x04 to register 0x00

## Additional info

- Sensor connection with Ardupilot based autopilots: https://ardupilot.org/copter/docs/common-rangefinder-lidarlite.html
- Sensor connection with PX4 based autopilots: https://docs.px4.io/main/en/sensor/lidar_lite.html
