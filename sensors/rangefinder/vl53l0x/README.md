# vl53l0x

The VL53L0X is a Time-of-Flight (ToF) laser-ranging module housed in the smallest package on the market today, providing accurate distance measurement whatever the target reflectances unlike conventional technologies. It can measure absolute distances up to 2m, setting a new benchmark in ranging performance levels, opening the door to various new applications.

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/vl53l0x.jpg?raw=true" alt="drawing" width="256">

|   |   |
| - | - |
| Reference | [Datasheet](https://www.st.com/resource/en/datasheet/vl53l0x.pdf) |
| Requred periphery | I2C |
| Unit tests | not yet |

## Usage

Installation:
1. Include the header file `vl53l0x.h` and source file `vl53l0x.c` in your C/C++ code.
2. Ensure that the required dependencies are installed, such as the STM32 HAL library or any other libraries specific to your STM32 development environment.

Usage:
1. During the boot of your application initialize I2C driver and then call `vl53l0xInit()`
2. Periodically (with rate 10 Hz) call `vl53l0xCollectData()`. If it returns `LIBPERIPH_OK`, then call `vl53l0xParseCollectedData()`.

## Additional info

- Sensor connection with Ardupilot based autopilots: https://ardupilot.org/copter/docs/common-vl53l0x-lidar.html
