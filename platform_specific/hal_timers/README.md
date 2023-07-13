## Hardware timers (PWM or input capture mode) HAL

The usage of hal_timers driver is self documented in [hal_timers.h header file](hal_timers.h). Please refer to it for details.

Typically, you need to work with only the following functions:

```c++
int8_t timerInit(Channel_t channel, TimerMode_t mode);
void timerSetPwmDuration(Channel_t channel, uint32_t pwm);
void timerSetPwmFrequency(Channel_t ch, uint16_t frequency_hz);
```

## stm32 usage

If you want to use stm32 platform, you should specify `LIBPERIPH_PLATFORM=stm32`.

It is expected to configure the periphery in STM32CubeMX.

You should enable TIMERS as shown below:

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/periphery/hal_timers/tim4_mode_and_configuration.png?raw=true" alt="drawing">

**Minimal usage example**

You should explicetely define which channels are you going to use, for example:

```
PIN_PB7_SET_TIM4_CH2=1
```

Then you need to initialize the peripheral and control it just with a few lines of code:

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

## Ubuntu usage

If you want to use Ubuntu platform, you should specify `LIBPERIPH_PLATFORM=ubuntu`.

Ubuntu platform might used either for unit tests or for SITL mode.

For unit tests example based on Ubuntu implementation please check [tests/periphery/test_timers.cpp](../../tests/periphery/test_timers.cpp).

In both ways you can implement your own version of `void ubuntuStm32TimerCallback(Channel_t ch)` function:

```c++
void ubuntuStm32TimerCallback(Channel_t ch) {
    printf("Update timer: idx=%i, mode=%i, duration=%i\n",
           ch,
           timerGetMode(ch),
           timerGetPwmDuration(ch));
}
```

By default, this fuction has a weak attribute and just print the updated state of the timer channel. It is called each time the timer update his state: initialization, updating duration or frequency.

Using this function you can emulate the real TIMER in a way you want.
