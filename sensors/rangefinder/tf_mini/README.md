# Benewake TFmini / TF-Luna driver

This driver is intended to be used for [Benewake](https://en.benewake.com/) TFmini and TF-Luna UART single-point short distance sensors based on the TOF (Time of Flight) principle.

These lidars mainly used for stable, accurate and high-frame-rate range detection.

The supported list of sensors are:

| Sensor      | Image | All supported protocols | Distance |
| ----------- | ----- | ----------------------- | -------- |
| TFmini-S / TFmini Plus | <img src="https://docs.px4.io/main/assets/img/tfmini_hero.d1a57ff6.jpg" alt="drawing" width="64"> | UART, I2C, I/O          | 0.1 - 12 meters |
| TF-Luna     | <img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/tf_mini.jpg?raw=true" alt="drawing" width="64"> | UART, I2C, I/O          | 0.2 - 8 meters |

## Details

|   |   |
| - | - |
| Reference | [TF-Luna datasheet](https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf) </br> [TFmini datasheet](https://cdn.sparkfun.com/assets/d/9/e/c/d/TFmini-I__C-Product_Manual_V1.1_EN.pdf) |
| Requred periphery | UART |
| Unit tests | [test_tf_mini.cpp](../../../tests/sensors/rangefinder/test_tf_mini.cpp) |

## Usage

Installation:
1. Include the header file `tf_mini.h` and source file `tf_mini.c` in your C/C++ code.
2. Ensure that the required dependencies are installed, such as the STM32 HAL library or any other libraries specific to your STM32 development environment.

Usage:
1. Call `tfMiniInit()` in the beginning of your program
2. Obtain data from UART/serial.
3. Call `tfParseRange` to parse the raw data.


## Additional info

- Sensor connection with Ardupilot based autopilots: https://ardupilot.org/copter/docs/common-benewake-tf02-lidar.html
- Sensor connection with PX4 based autopilots: https://docs.px4.io/main/en/sensor/tfmini.html