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

## Sensors types

Lightware rangefinders:

1. SF10/a (25m 32Hz)
    - min_distance: 0.01
    - max_distance: 25.0
2. SF10/b (50m 32Hz)
    - min_distance: 0.01f
    - max_distance: 50.0f
3. SF10/c (100m 16Hz)
    - min_distance: 0.01
    - max_distance: 100.0
4. SF11/c (120m 20Hz)
    - min_distance: 0.2f
    - max_distance: 120.0
5. SF/LW20/b (50m 48-388Hz)
    - min_distance: 0.2
    - max_distance: 50.0
6. **SF/LW20/c (100m 48-388Hz)**
    - min_distance: 0.2
    - max_distance: 100.0
7. SF/LW30/d (200m 49-20'000Hz)
    - min_distance: 0.2
    - max_distance: 200.0

## Additional info

- Sensor connection with Ardupilot based autopilots: https://ardupilot.org/copter/docs/common-lightware-sf10-lidar.html
- Sensor connection with PX4 based autopilots: https://docs.px4.io/main/en/sensor/sfxx_lidar.html
