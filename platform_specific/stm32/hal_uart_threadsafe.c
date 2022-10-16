/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_uart_threadsafe.h"
#include "config.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "main.h"

#ifdef USE_THREAD_SAFE_UART
    static xSemaphoreHandle dma_tx_sem;
    static StaticSemaphore_t dma_tx_sem_buf;
    static BaseType_t pxHigherPriorityTaskWoken = pdTRUE;
#endif

static xSemaphoreHandle dma_rx_sem;
static StaticSemaphore_t dma_rx_sem_buf;

#ifdef HAL_UART_MODULE_ENABLED
    static BaseType_t dma_rx_sem_task_woken = pdTRUE;
#endif

int8_t uartInitRxDmaWithSem(uint8_t buffer[], uint16_t size) {
    dma_rx_sem = xSemaphoreCreateBinaryStatic(&dma_rx_sem_buf);
    if (dma_rx_sem == NULL) {
        return STATUS_ERROR;
    } else {
        xSemaphoreGive(dma_rx_sem);
    }

    return uartInitRxDma(buffer, size);
}

bool uartWaitUntilReceiveIsComplete(uint32_t timeout_ms) {
    if (dma_rx_sem == NULL) {
        return false;
    } else if (xSemaphoreTake(dma_rx_sem, timeout_ms) == pdTRUE) {
        return true;
    } else {
        return false;
    }
}


int8_t uartInitTxDmaThreadSafe() {
#if defined(HAL_UART_MODULE_ENABLED) && defined(USE_THREAD_SAFE_UART)
    if (dma_tx_sem != NULL) {
        return STATUS_ERROR;
    }
    dma_tx_sem = xSemaphoreCreateBinaryStatic(&dma_tx_sem_buf);
    if (dma_tx_sem == NULL) {
        return STATUS_ERROR;
    } else {
        xSemaphoreGive(dma_tx_sem);
    }
    return STATUS_OK;
#else
    return STATUS_ERROR;
#endif
}

HAL_StatusTypeDef uartTransmitDmaThreadSafe(uint8_t buffer[], size_t size) {
#ifdef USE_THREAD_SAFE_UART
    if (dma_tx_sem == NULL) {
        return HAL_ERROR;
    } else if (xSemaphoreTake(dma_tx_sem, 100) == pdTRUE) {
        HAL_StatusTypeDef tx_result = uartTransmitDma(buffer, size);
        if (tx_result != HAL_OK) {
            if (dma_tx_sem != NULL) {
                xSemaphoreGiveFromISR(dma_tx_sem, &pxHigherPriorityTaskWoken);
            } else {
                asm("NOP");  ///< handle critical error
            }
        }
        return tx_result;
    } else {
        return HAL_ERROR;
    }
#else
    UNUSED(buffer);
    UNUSED(size);
    return HAL_ERROR;
#endif
}

void uartRxDmaCallback() {
#ifdef HAL_UART_MODULE_ENABLED
    if (dma_rx_sem != NULL) {
        xSemaphoreGiveFromISR(dma_rx_sem, &dma_rx_sem_task_woken);
    }
#endif
}

void uartTxDmaCallback() {
#ifdef USE_THREAD_SAFE_UART
    if (dma_tx_sem != NULL) {
        xSemaphoreGiveFromISR(dma_tx_sem, &pxHigherPriorityTaskWoken);
    } else {
        asm("NOP");  ///< handle critical error
    }
#endif
}
