/**
 * @file hal_timers.h
 * @author d.ponomarev
 * @date Aug 11, 2022
 */

#ifndef TESTS_MOCK_HAL_TIMERS_H_
#define TESTS_MOCK_HAL_TIMERS_H_

#include <stdint.h>

typedef enum {
    TIM_CH_BEGIN=0,
    TIM_CH_AMOUNT,
} Channel_t;

typedef enum {
    TIMER_MODE_CAPTURE = 0,
    TIMER_MODE_PWM,
    TIMER_MODES_AMOUNT,

    TIMER_NOT_CONFIGURED = 0xFF
} TimerMode_t;


int8_t timerInit(Channel_t channel, TimerMode_t mode);
TimerMode_t timerGetMode(Channel_t channel);
void timerSetPwmDuration(Channel_t channel, uint32_t pwm);
int32_t timerGetPwmDuration(Channel_t channel);

#endif  // TESTS_MOCK_HAL_TIMERS_H_
