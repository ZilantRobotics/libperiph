/**
 * @file hal_timers.h
 * @author d.ponomarev
 * @date Aug 11, 2022
 */

#ifndef TESTS_MAIN_H_
#define TESTS_MAIN_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int value;
} TIM_HandleTypeDef;

int8_t HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef*, uint32_t, uint32_t*, uint32_t);
int8_t HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef*, uint32_t);

#ifdef __cplusplus
}
#endif

#define HAL_OK 0

#endif  // TESTS_MAIN_H_
