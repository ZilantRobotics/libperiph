# libperiph ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/arm_build.yml/badge.svg) ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/code_style.yml/badge.svg) ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/unit_tests.yml/badge.svg) [![Coverage](https://sonarcloud.io/api/project_badges/measure?project=ZilantRobotics_libperiph&metric=coverage)](https://sonarcloud.io/summary/new_code?id=ZilantRobotics_libperiph) [![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=ZilantRobotics_libperiph&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=ZilantRobotics_libperiph) [![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=ZilantRobotics_libperiph&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=ZilantRobotics_libperiph)

libperiph is a collection of open source avionics drivers written in C.

It contains a minimal set of drivers needed for a UAV application from a simple quadcopter (barometer, magnetometer, GNSS, actuators) to complex VTOL (airspeed, rangefinders, current sensor, navigation lights), etc.

The drivers are designed to be platform independent. You can port them to stm32, arduino or anything else that has the required interfaces. Some sample platform implementations have been written to make it easy to get started: HAL based peripheral drivers for stm32 and dummy drivers for Ununtu to run them in SITL mode. Please see [platform_specific](platform_specific) for details.

The structural relationship between the drivers is shown in the figure below:

```
+-------+------+ +-------+------+ +-------+------+
|    sensors   | |    devices   | |    common    |
+--------------+ +--------------+ +--------------+
                         |
+-------+----------------+---------------+-------+
|   Platform specific interface (header files)   |
+-------+----------------+-----------------------+
            |                          |
+-------+-------+-----+    +-------+-------+-----+
| STM32Cube HAL based |    |    Ubuntu SITL      |
|   Implementation    |    |   Implementation    |
+-------+-------+-----+    +-------+-------+-----+
```

The library has been tested on:
- stm32f103,
- stm32fg0,
- stm32f3discovery board,
- stm32f767 nucleo board.


**Drivers for a minimal quadcopter application**

| Driver                                                    | HAL required |
| --------------------------------------------------------- | ------------ |
| [sensors/bmp280](sensors/barometer)                       | i2c          |
| [sensors/gps](sensors/gps)                                | -            |
| [sensors/mag_hmc5883l](sensors/magnetometer)              | i2c          |
| [sensors/mag_rm3100](sensors/magnetometer)                | spi          |
| [sensors/temperature_sensor](sensors/temperature_sensor)  | -            |
| [devices/servos](devices/servos)                          | pwm          |
| [platform_specific](platform_specific)                    |

**Drivers required for a VTOL application:**

| Driver                                                                | HAL required |
| --------------------------------------------------------------------- | ------------ |
| [sensors/acs712](sensors/current_sensor)                              | -            |
| [sensors/ms4252do](sensors/differential_pressure)                     | -            |
| [sensors/rangefinder/garmin_lite](sensors/rangefinder/garmin_lite)    | i2c          |
| [sensors/rangesensor/sf1xx](sensors/rangefinder/sf1xx)                | refactor me  |
| [sensors/rangesensor/tf_luna](sensors/rangefinder/tf_luna)            | -            |
| [sensors/rangesensor/vl53l0x](sensors/rangefinder/vl53l0x)            | -            |

**Auxiliary drivers**

| Driver                                    | HAL required |
| ----------------------------------------- | ------------ |
| [devices/rgb_leds](devices/rgb_leds)      | pwm dma      |
| sensors/encoder                           | gpio         |
| [sensors/esc/alpha](sensors/esc)          | -            |
| [sensors/esc/flame](sensors/esc)          | -            |
| [sensors/esc/thunder](sensors/esc)        | -            |
| sensors/mpu9250                           | i2c          |
| sensors/rangefinder/srf_05                | refactor me  |

## Usage

You can either use the repository as submodule in your project or copy only the required drivers.

Typically, each driver has `.c` and `.h` files. Include them into your project. For specific usage
details please refer to the corresponded README.md file in the driver folder.

## For developers

**Test coverage**

Drivers are covered with unit tests with [gtest](https://github.com/google/googletest). The current coverage is exstimated as [![Coverage](https://sonarcloud.io/api/project_badges/measure?project=ZilantRobotics_libperiph&metric=coverage)](https://sonarcloud.io/summary/new_code?id=ZilantRobotics_libperiph). To run unit tests you can type:

```bash
make unit_tests
```

If you want to estimate coverage manually, you need to type:

```bash
make coverage
```

**Code style**

The code style is checked by [astyle](https://astyle.sourceforge.net/) and [cpplint](https://github.com/cpplint/cpplint).

## LICENSE

This software is distributed under [MPL 2.0](license) license conditions.
