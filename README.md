# libperiph

This repository has an example of periphery and different device drivers implementation.

It was tested on:
- stm32f3discovery board,
- dsPIC33F,
- stm32f767 nucleo board,
- stm32f103.

| Periphery                         | Required HAL | tests |
| --------------------------------- | ------------ | ----- |
| devices/esp8266                   | uart         | build |
| devices/motors                    | chibios      | -     |
| devices/servos                    | pwm          | build + unit tests |
| devices/seven_segments_indicators | gpio         | build |
| devices/ws2812                    | pwm dma      | -     |
| sensors/acs712                    | -            | build |
| sensors/baro_bmp280               | i2c          | build |
| sensors/encoder                   | gpio         | build |
| sensors/esc_flame                 | -            | build + unit tests |
| sensors/mag_hmc5883l              | i2c          | build |
| sensors/mag_rm3100                | spi          | build |
| sensors/mpu9250                   | i2c          | build |
| sensors/ms4252do                  | -            | build + unit tests |
| sensors/rangefinder/garmin_lite   | i2c          | build |
| sensors/rangefinder/srf_05        | refactor me  | build |
| sensors/rangesensor/sf1xx         | refactor me  | build |
| sensors/rangesensor/tf_luna       | -            | build + unit tests |
| sensors/rangesensor/vl53l0x       | -            |  |
| sensors/temperature_sensor        | -            | build + unit tests |
| sensors/ublox                     | -            | build + unit tests |


## HAL requirements

Some drivers do not require any HAL specific functions, but other expected to have an outside implementation of few function prototypes.

**hal_i2c**

```c++
int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len);
int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len);
```

**hal_spi**

```c++
int8_t spiPerformTransaction(uint8_t* tx, uint8_t* rx, uint8_t size);
```

**hal_uart**

```c++
int8_t uartInitRx(UartInstance_t instance, UartType_t, uint8_t buffer[], uint16_t size);
size_t uartLastRecvIndex(UartInstance_t instance);
uint8_t* uartRxDmaPop(UartInstance_t instance);

int8_t uartTransmit(UartInstance_t instance, uint8_t buffer[], size_t size);
```

**hal_tim**

> in process

## LICENSE

This software is distributed under [MPL 2.0](license) license conditions.
