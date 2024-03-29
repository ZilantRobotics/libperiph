# libperiph ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/arm_build.yml/badge.svg) ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/code_style.yml/badge.svg) ![badge](https://github.com/PonomarevDA/libperiph/actions/workflows/unit_tests.yml/badge.svg) [![Coverage](https://sonarcloud.io/api/project_badges/measure?project=ZilantRobotics_libperiph&metric=coverage)](https://sonarcloud.io/summary/new_code?id=ZilantRobotics_libperiph) [![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=ZilantRobotics_libperiph&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=ZilantRobotics_libperiph) [![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=ZilantRobotics_libperiph&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=ZilantRobotics_libperiph)

**Purpose**

libperiph is a collection of open source avionics drivers. It contains a minimal set of peripheral drivers needed for a UAV application from a simple quadcopter (barometer, magnetometer, GNSS, servos and ESC, temperature sensor) to complex VTOL (airspeed, rangefinders, current sensor, navigation lights), etc. The library's drivers are intended to be used in real-time embedded applications.

**Minimal requirements**

The codebase is implemented in C99/C11.

The library was tested on stm32f103 (128 Kbytes) and stm32g0 (512 Kbytes). These hardware can be considered as minimal required.

**Design**

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

Sensors:

| Driver                                                        | HAL required |
| ------------------------------------------------------------- | ------------ |
| [Barometer bmp280](sensors/barometer)                         | i2c          |
| [Current sensor acs712](sensors/current_sensor)               | ADC          |
| [Diff pressure ms4252do](sensors/differential_pressure)       | i2c          |
| [ESC Alpha](sensors/esc)                                      | UART         |
| [ESC Flame](sensors/esc)                                      | UART         |
| [ESC Thunder](sensors/esc)                                    | UART         |
| [GNSS UBLOX](sensors/gps)                                     | UART         |
| [Magnetometer hmc5883l](sensors/magnetometer)                 | i2c          |
| [Magnetometer rm3100](sensors/magnetometer)                   | spi          |
| [Temperature sensor](sensors/temperature_sensor)              | ADC          |
| [Rangefinder garmin lite](sensors/rangefinder/garmin_lite)    | i2c          |
| [Rangefinder sf1xx](sensors/rangefinder/sf1xx)                | i2c          |
| [Rangefinder tf_mini](sensors/rangefinder/tf_mini)            | UART         |
| [Rangefinder vl53l0x](sensors/rangefinder/vl53l0x)            | i2c          |
| Encoder as5600                                                | gpio         |
| Inclinometer mpu9250                                          | i2c          |

Other (devices, platform specific and common):

| Driver                                                        | HAL required |
| ------------------------------------------------------------- | ------------ |
| [Servos](devices/servos)                                      | pwm          |
| [RGB LEDs](devices/rgb_leds)                                  | pwm dma      |
| [Platform specific](platform_specific)                        |


## Usage

You can either use the repository as a submodule in your project or copy only the required drivers.

Typically, each driver has `.c` and `.h` files. Include them into your project. For specific usage
details please refer to the corresponding README.md file in the driver folder.

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

This software is distributed under [MPL 2.0](LICENSE) license conditions.

## Acknowledgements

This project has been supported by funds from The Foundation for Assistance to Small Innovative Enterprises (FASIE). Moreover, we are honored to be distinguished as laureates of the "Digital Technologies Code" competition, under the federal project "Digital Technologies". We express our profound gratitude for their invaluable support and endorsement.
