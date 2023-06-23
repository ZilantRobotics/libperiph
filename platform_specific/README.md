## HAL drivers

Some drivers do not require any HAL specific functions, but other expected to have an outside implementation of few function prototypes.

**hal_adc**

```c++
int8_t adcInitDma(uint8_t num_of_channels);
int8_t adcGetAll(uint16_t adc_measurements[]);
uint16_t adcGet(uint8_t rank);
```

**hal_i2c**

```c++
int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len);
int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len);
```

**hal_spi**

```c++
int8_t spiPerformTransaction(uint8_t* tx, uint8_t* rx, uint8_t size);
```

**hal_tim**

```c++
int8_t timerInit(Channel_t channel, TimerMode_t mode);
void timerSetPwmDuration(Channel_t channel, uint32_t pwm);
void timerSetPwmFrequency(Channel_t ch, uint16_t frequency_hz);
```

**hal_uart**

```c++
int8_t uartInitRx(UartInstance_t instance, UartType_t, uint8_t buffer[], uint16_t size);
size_t uartGetLastReceivedIndex(UartInstance_t instance);
uint8_t* uartRxDmaPop(UartInstance_t instance);

int8_t uartTransmit(UartInstance_t instance, uint8_t buffer[], size_t size);
```
