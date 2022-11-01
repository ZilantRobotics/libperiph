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
#include "hal_uart.h"
#include "libperiph_common.h"

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

int8_t tsUartInitRx(uint8_t buffer[], uint16_t size) {
    dma_rx_sem = xSemaphoreCreateBinaryStatic(&dma_rx_sem_buf);
    if (dma_rx_sem == NULL) {
        return STATUS_ERROR;
    } else {
        xSemaphoreGive(dma_rx_sem);
    }

    return uartInitRxDma(UART_FIRST, buffer, size);
}

bool tsUartWaitUntilReceiveIsComplete(uint32_t timeout_ms) {
    if (dma_rx_sem == NULL) {
        return false;
    }

    return (xSemaphoreTake(dma_rx_sem, timeout_ms) == pdTRUE) ? true : false;
}

uint8_t* tsUartPopRxDma() {
    return uartRxDmaPop();
}


int8_t tsUartInitTx() {
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

int8_t tsUartTransmit(uint8_t buffer[], size_t size) {
#ifdef USE_THREAD_SAFE_UART
    if (dma_tx_sem == NULL) {
        return STATUS_ERROR;
    } else if (xSemaphoreTake(dma_tx_sem, 100) == pdTRUE) {
        int8_t tx_result = uartTransmitDma(buffer, size);
        if (tx_result != 0) {
            if (dma_tx_sem != NULL) {
                xSemaphoreGiveFromISR(dma_tx_sem, &pxHigherPriorityTaskWoken);
            } else {
                asm("NOP");  ///< handle critical error
            }
        }
        return (tx_result == HAL_OK) ? 0 : -1;
    } else {
        return STATUS_ERROR;
    }
#else
    UNUSED(buffer);
    UNUSED(size);
    return STATUS_ERROR;
#endif
}

void tsUartRxDmaCallback() {
#ifdef HAL_UART_MODULE_ENABLED
    if (dma_rx_sem != NULL) {
        xSemaphoreGiveFromISR(dma_rx_sem, &dma_rx_sem_task_woken);
    }
#endif
}

void tsUartTxDmaCallback() {
#ifdef USE_THREAD_SAFE_UART
    if (dma_tx_sem != NULL) {
        xSemaphoreGiveFromISR(dma_tx_sem, &pxHigherPriorityTaskWoken);
    } else {
        asm("NOP");  ///< handle critical error
    }
#endif
}
