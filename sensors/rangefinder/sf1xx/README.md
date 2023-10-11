# LightWare SF20 and LW20 rangefinders

The Lightware SF20 and LW20 are small but long range (100m) and accurate range finders.

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/lw20.jpg?raw=true" alt="drawing" width="256">

|   |   |
| - | - |
| Reference | [Datasheet](https://www.mouser.com/datasheet/2/321/28055-LW20-SF20-LiDAR-Manual-Rev-7-1371848.pdf) |
| Requred periphery | I2C |
| Unit tests | not yet |

## Usage

Installation:
1. Include the header file `sf1xx.h` and source file `sf1xx.c` in your C/C++ code.
2. Ensure that the required dependencies are installed, such as the STM32 HAL library or any other libraries specific to your STM32 development environment.

Usage:
1. During the boot of your application initialize I2C driver and then call `sf1xxInit()`
2. Periodically (with rate 10 Hz) call `sf1xxCollectData()`. If it returns `LIBPERIPH_OK`, then call `sf1xxParseCollectedData()`.

## Additional info

- Sensor connection with Ardupilot based autopilots: https://ardupilot.org/copter/docs/common-lightware-sf10-lidar.html
- Sensor connection with PX4 based autopilots: https://docs.px4.io/main/en/sensor/sfxx_lidar.html
