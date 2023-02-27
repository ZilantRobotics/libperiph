# libperiph

This repository has an example of periphery and different device drivers implementation.

It was tested on:
- stm32f3discovery board,
- dsPIC33F,
- stm32f767 nucleo board,
- stm32f103.

**Part 1.**

| Periphery                                 | Required HAL | Arm build | Test coverage |
| ----------------------------------------- | ------------ | --------- | ------------- |
| sensors/baro_bmp280                       | i2c          | +         | 41.43% of 70  |
| sensors/ublox                             | -            | +         | 94.29% of 105 |
| sensors/mag_hmc5883l                      | i2c          | +         | 50.00% of 34  |
| sensors/mag_rm3100                        | spi          | +         | 57.97% of 69  |
| sensors/temperature_sensor                | -            | +         | 100.00% of 3  |
| devices/servos                            | pwm          | +         | 46.09% of 115 |
| [platform_specific](platform_specific)    |              |

Summary: Lines executed:65.71% of 452

**Part 2.**

| Periphery                                 | Required HAL | Arm build | Test coverage |
| ----------------------------------------- | ------------ | --------- | ------------- |
| sensors/acs712                            | -            | +         | 
| sensors/ms4252do                          | -            | +         | 78.26% of 23 |
| sensors/rangefinder/garmin_lite           | i2c          | -         |
| sensors/rangesensor/sf1xx                 | refactor me  | -         |
| sensors/rangesensor/tf_luna               | -            | +         | 88.00% of 25 |
| sensors/rangesensor/vl53l0x               | -            | +         |

**Auxiliary**

| Periphery                                 | Required HAL | Arm build | Test coverage |
| ----------------------------------------- | ------------ | --------- | ------------- |
| devices/esp8266                           | uart         | +         |
| devices/motors                            | chibios      | -         |
| devices/seven_segments_indicators         | gpio         | +         |
| [devices/rgb_leds](devices/rgb_leds)      | pwm dma      | -         | 17.86% of 28 |
| sensors/encoder                           | gpio         | -         |
| sensors/esc_flame                         | -            | +         | 100.00% of 31|
| sensors/mpu9250                           | i2c          | +         |
| sensors/rangefinder/srf_05                | refactor me  | +         |

## LICENSE

This software is distributed under [MPL 2.0](license) license conditions.
