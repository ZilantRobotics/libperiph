# libperiph ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/arm_build.yml/badge.svg) ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/code_style.yml/badge.svg) ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/unit_tests.yml/badge.svg)

libperiph is a collection of open source aviation onloard electronics drivers writen in C.

It has a minimal set of drivers required for an UAV application, such as: barometer, magnetometer, gnss, actuators and so on.

The drivers are designed to be platform independent. You can port them on stm32, arduino or anything else that has a requried interfaces. To make it easire to start, a few platform expample implementations were written: HAL based periphery drivers for stm32 and dummy drivers for ununtu. Please check [platform_specific](platform_specific) for details.

It was tested on:
- stm32f3discovery board,
- stm32f767 nucleo board,
- stm32f103,
- stm32fg0.

The structural relationship of the drivers to each other is shown in the figure below:

![](https://github.com/PonomarevDA/libperiph/blob/docs/assets/structure.png?raw=true)

Here orange blocks means periphery drivers, blue - sensors, green - devices.

**Part 1. Here are drivers required for minimal quadcopter application:**

| Driver                                                    | Required HAL | Arm build | Test coverage |
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

| Driver                                    | Required HAL | Arm build | Test coverage |
| ----------------------------------------- | ------------ | --------- | ------------- |
| [sensors/acs712](sensors/current_sensor)  | -            | +         | 
| sensors/ms4252do                          | -            | +         | 78.26% of 23 |
| sensors/rangefinder/garmin_lite           | i2c          | -         |
| sensors/rangesensor/sf1xx                 | refactor me  | -         |
| sensors/rangesensor/tf_luna               | -            | +         | 88.00% of 25 |
| sensors/rangesensor/vl53l0x               | -            | +         |

**Auxiliary**

| Driver                                    | Required HAL | Arm build | Test coverage |
| ----------------------------------------- | ------------ | --------- | ------------- |
| [devices/rgb_leds](devices/rgb_leds)      | pwm dma      | -         | 17.86% of 28 |
| sensors/encoder                           | gpio         | -         |
| sensors/esc_flame                         | -            | +         | 100.00% of 31|
| sensors/mpu9250                           | i2c          | +         |
| sensors/rangefinder/srf_05                | refactor me  | +         |

## LICENSE

This software is distributed under [MPL 2.0](license) license conditions.
