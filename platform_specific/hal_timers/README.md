## Hardware timers (PWM or input capture mode) HAL

The usage of hal_timers driver is self documented in [hal_timers.h header file](hal_timers.h). Please refer to it for details.

Typically, you need to work with the following functions:

```c++
int8_t timerInit(Channel_t channel, TimerMode_t mode);
void timerSetPwmDuration(Channel_t channel, uint32_t pwm);
void timerSetPwmFrequency(Channel_t ch, uint16_t frequency_hz);
```

Minimal usage example:

Add define:

```
PIN_PB7_SET_TIM4_CH2=1
```

```c++
#include "hal_timers.h"

// Let's control PWM for PB7
const Channel_t channel = PIN_PB7_TIM4_CH2;
const TimerMode_t mode = TIMER_MODE_PWM;

// Initialization
int8_t res = timerInit(channel, mode);

// Control
timerSetPwmDuration(channel, 1000);
timerSetPwmFrequency(channel, 50);
```
