# libperiph ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/arm_build.yml/badge.svg) ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/code_style.yml/badge.svg) ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/unit_tests.yml/badge.svg)

libperiph is a collection of open source avionics drivers written in C.

It contains a minimal set of drivers needed for a UAV application, such as: barometer, magnetometer, GNSS, actuators and so on.

The drivers are designed to be platform independent. You can port them to stm32, arduino or anything else that has the required interfaces. Some sample platform implementations have been written to make it easy to get started: HAL based peripheral drivers for stm32 and dummy drivers for Ununtu. Please see [platform_specific](platform_specific) for details.

It has been tested on:
- stm32f3discovery board,
- stm32f767 nucleo board,
- stm32f103,
- stm32fg0.

The structural relationship between the drivers is shown in the figure below:

![](https://github.com/PonomarevDA/libperiph/blob/docs/assets/structure.png?raw=true)

Orange blocks are peripheral drivers, blue - sensors, green - devices.

**Part 1. Here are the drivers needed for a minimal quadcopter application:**

| Driver                                                    | HAL required | Arm build | Test coverage |
| --------------------------------------------------------- | ------------ | --------- | ------------- |
| [sensors/bmp280](sensors/barometer)                       | i2c          | +         | 96.77% of 62  |
| [sensors/gps](sensors/gps)                                | -            | +         | 94.29% of 105 |
| [sensors/mag_hmc5883l](sensors/magnetometer)              | i2c          | +         | 50.00% of 34  |
| [sensors/mag_rm3100](sensors/magnetometer)                | spi          | +         | 57.97% of 69  |
| [sensors/temperature_sensor](sensors/temperature_sensor)  | -            | +         | 100.00% of 3  |
| [devices/servos](devices/servos)                          | pwm          | +         | 46.09% of 230 |
| [platform_specific](platform_specific)                    |              |

Summary: Lines executed:73.87% of 444

**Part 2. Here are drivers required for a VTOL application:**

| Driver                                                                | HAL required | Arm build | Test coverage |
| --------------------------------------------------------------------- | ------------ | --------- | ------------- |
| [sensors/acs712](sensors/current_sensor)                              | -            | +         |
| [sensors/ms4252do](sensors/differential_pressure)                     | -            | +         | 78.26% of 23 |
| [sensors/rangefinder/garmin_lite](sensors/rangefinder/garmin_lite)    | i2c          | -         |
| [sensors/rangesensor/sf1xx](sensors/rangefinder/sf1xx)                | refactor me  | -         |
| [sensors/rangesensor/tf_luna](sensors/rangefinder/tf_luna)            | -            | +         | 88.00% of 25 |
| [sensors/rangesensor/vl53l0x](sensors/rangefinder/vl53l0x)            | -            | +         |

**Auxiliary**

| Driver                                    | HAL required | Arm build | Test coverage |
| ----------------------------------------- | ------------ | --------- | ------------- |
| [devices/rgb_leds](devices/rgb_leds)      | pwm dma      | -         | 17.86% of 28 |
| sensors/encoder                           | gpio         | -         |
| [sensors/esc/flame](sensors/esc)          | -            | +         | 100.00% of 31|
| [sensors/esc/thunder](sensors/esc)        | -            | -         | 100.00% of 63 |
| sensors/mpu9250                           | i2c          | +         |
| sensors/rangefinder/srf_05                | refactor me  | +         |

## LICENSE

This software is distributed under [MPL 2.0](license) license conditions.
