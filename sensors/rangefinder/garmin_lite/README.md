# Garmin lite

The Garmin / PulsedLight LIDAR-Lite rangefinder is a low-cost optical distance measurement solution with a 40m range under most operating conditions, low power consumption, and small form factor.

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/rangefinder/garmin_lite_v3.jpg?raw=true" alt="drawing" width="256">

|   |   |
| - | - |
| Reference | [Datasheet](https://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf) |
| Requred periphery | I2C |
| Unit tests | not yet |

## Implementation details

The simplest method of obtaining measurement results from the I2C interface is as follows:
1. Write 0x04 to register 0x00
2. Read register 0x01. Repeat until bit 0 (LSB) goes low
3. Read two bytes from 0x8f (0x0f and 0x10) to obtain 16-bit measured distance in centimeters.

Slightly optimized algorithm:
1. Read two bytes from 0x8f (0x0f and 0x10) to obtain 16-bit measured distance in centimeters
2. Write 0x04 to register 0x00
