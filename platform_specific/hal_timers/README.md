## Usage

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
