/**
 * @file hal_timers_mock.c
 * @author d.ponomarev
 * @date Aug 11, 2022
 */

#include "hal_timers.h"

int8_t timerInit(Channel_t channel, TimerMode_t mode) {return 0;}
TimerMode_t timerGetMode(Channel_t channel) {return TIMER_MODE_PWM;}
void timerSetPwmDuration(Channel_t channel, uint32_t pwm) {}
int32_t timerGetPwmDuration(Channel_t channel) {return 900;}
