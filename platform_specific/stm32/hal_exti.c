/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_exti.h"
#include "main.h"
#include "cmsis_os.h"
#include "config.h"
#include "algorithms.h"
#include "semphr.h"


#define NUM_OF_SEMAPHORES       2
#define NUM_OF_EXTI_CHANNELS    16
#define PIN_BOUNCE_PERIOD_MS    20
#define SEM_IDX_FOR_EXTI_12_15  0
#define SEM_IDX_FOR_EXTI_0_11   1
#define EXTI_12_15_MIN          12
#define EXTI_12_15_MAX          15
#define EXTI_0_11_MIN           0
#define EXTI_0_11_MAX           11

typedef struct {
    uint32_t time;
    bool is_interrupted;
} ExtiChannel_t;

typedef struct {
    xSemaphoreHandle sem;
    StaticSemaphore_t buf;
} ExtiSemaphore_t;


static ExtiSemaphore_t semaphores[NUM_OF_SEMAPHORES];
static ExtiChannel_t exti_channels[NUM_OF_EXTI_CHANNELS] = {0};
static bool is_already_inited = false;


static IRQn_Type extiGetIrqTypeByPinNumber(uint16_t pin_num);
static void extiEnableInterruptForParticularPin(uint16_t pin_num);
static void extiDisableInterruptForParticularPin(uint16_t pin_num);
static uint8_t extiGetSemIdxByPinNumber(uint16_t pin_num);


int8_t extiInit() {
    if (is_already_inited) {
        return STATUS_ERROR;
    }
    for (uint_fast8_t sem_idx = 0; sem_idx < NUM_OF_SEMAPHORES; sem_idx++) {
        semaphores[sem_idx].sem = xSemaphoreCreateBinaryStatic(&semaphores[sem_idx].buf);
        if (semaphores[sem_idx].sem == NULL) {
            return STATUS_ERROR;
        }
    }

    is_already_inited = true;
    return STATUS_OK;
}

bool extiIsInited() {
    return is_already_inited;
}

IRQn_Type extiGetIrqTypeByPinNumber(uint16_t pin_num) {
    IRQn_Type irq_type;
#ifdef STM32F103xB
    if (pin_num >= 10 && pin_num <= 15) {
        irq_type = EXTI15_10_IRQn;
    } else if (pin_num >= 5 && pin_num <= 9) {
        irq_type = EXTI9_5_IRQn;
    } else if (pin_num == 4) {
        irq_type = EXTI4_IRQn;
    } else if (pin_num == 3) {
        irq_type = EXTI3_IRQn;
    } else if (pin_num == 2) {
        irq_type = EXTI2_IRQn;
    } else if (pin_num == 1) {
        irq_type = EXTI1_IRQn;
    } else if (pin_num == 0) {
        irq_type = EXTI0_IRQn;
    } else {
        ///< handle error input argument here
        irq_type = EXTI0_IRQn;
    }
#else
    irq_type = 0;
#endif
    return irq_type;
}

bool extiWaitForInterrupt_12_15(uint16_t block_time_ms) {
    if (xSemaphoreTake(semaphores[SEM_IDX_FOR_EXTI_12_15].sem, block_time_ms) == pdTRUE) {
        for (uint16_t pin_num = EXTI_12_15_MIN; pin_num <= EXTI_12_15_MAX; pin_num++) {
            if (exti_channels[pin_num].is_interrupted) {
                extiEnableInterruptForParticularPin(pin_num);
                osDelay(PIN_BOUNCE_PERIOD_MS);
                return true;
            }
        }
        asm("NOP");  ///< handle critical error here
    }
    return false;
}

bool extiWaitForInterrupt_0_11(uint16_t block_time_ms) {
    if (xSemaphoreTake(semaphores[SEM_IDX_FOR_EXTI_0_11].sem, block_time_ms) == pdTRUE) {
        for (uint16_t pin_num = EXTI_0_11_MIN; pin_num <= EXTI_0_11_MAX; pin_num++) {
            if (exti_channels[pin_num].is_interrupted) {
                extiEnableInterruptForParticularPin(pin_num);
                osDelay(PIN_BOUNCE_PERIOD_MS);
                return true;
            }
        }
        asm("NOP");  ///< handle critical error here
    }
    return false;
}

uint8_t extiGetSemIdxByPinNumber(uint16_t pin_num) {
    return (pin_num >= EXTI_12_15_MIN) ? SEM_IDX_FOR_EXTI_12_15 : SEM_IDX_FOR_EXTI_0_11;
}

void extiEnableInterruptForParticularPin(uint16_t pin_num) {
    IRQn_Type irq_type = extiGetIrqTypeByPinNumber(pin_num);
    __HAL_GPIO_EXTI_CLEAR_IT(1 << pin_num);
    NVIC_ClearPendingIRQ(irq_type);
    HAL_NVIC_EnableIRQ(irq_type);
}

void extiDisableInterruptForParticularPin(uint16_t pin_num) {
    uint8_t IRQn_Type = extiGetIrqTypeByPinNumber(pin_num);
    static BaseType_t pxHigherPriorityTaskWoken = pdTRUE;

    HAL_NVIC_DisableIRQ(IRQn_Type);
    exti_channels[pin_num].time = HAL_GetTick();
    exti_channels[pin_num].is_interrupted = true;
    uint8_t sem_idx = extiGetSemIdxByPinNumber(pin_num);
    xSemaphoreGiveFromISR(semaphores[sem_idx].sem, &pxHigherPriorityTaskWoken);
}

void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin) {
    int8_t pin_num = getNumberOfHigherBitU16(gpio_pin);
    if (pin_num == STATUS_ERROR) {
        asm("NOP");  ///< handle critical error here
    }
    extiDisableInterruptForParticularPin(pin_num);
}
