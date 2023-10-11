# Benewake TF Luna driver

TF-Luna is a single-point ranging LiDAR, based on the TOF (Time of Flight) principle. It is mainly used for stable, accurate and high-frame-rate range detection. The TF-Luna has a range of 8m indoor, and 3m outdoor and weighs 5g. The sensor must be connected to a UART/serial bus.

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/tf_luna.jpg?raw=true" alt="drawing" width="256">

|   |   |
| - | - |
| Reference | [Datasheet](https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf) |
| Requred periphery | UART |
| Unit tests | [test_tf_luna.cpp](../../../tests/sensors/rangefinder/test_tf_luna.cpp) |

## Usage

Installation:
1. Include the header file `tf_luna.h` and source file `tf_luna.c` in your C/C++ code.
2. Ensure that the required dependencies are installed, such as the STM32 HAL library or any other libraries specific to your STM32 development environment.

Usage:
1. Call `tfLunaInit()` in the beginning of your program
2. Obtain data from UART/serial.
3. Call `tfParseRange` to parse the raw data.


## Additional info

- Sensor connection with Ardupilot based autopilots: https://ardupilot.org/copter/docs/common-benewake-tf02-lidar.html
- Sensor connection with PX4 based autopilots: https://docs.px4.io/main/en/sensor/tfmini.html