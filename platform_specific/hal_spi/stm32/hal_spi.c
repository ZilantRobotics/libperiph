/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <hal_spi.h>
#include <string.h>
#include "main.h"
#include "libperiph_common.h"


#define MEASUREMENT_DELAY                   10


#ifdef HAL_SPI_MODULE_ENABLED
    extern SPI_HandleTypeDef hspi1;
    SPI_HandleTypeDef* hspi_ptr = &hspi1;
#else
    void* hspi_ptr = NULL;
#endif

int8_t spiPerformTransaction(uint8_t* tx, uint8_t* rx, uint8_t size) {
#ifdef SPI_SS_GPIO_Port
    memset(rx, 0x00, size);
    HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_ptr, tx, rx, size, MEASUREMENT_DELAY);
    HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_SET);
    return (status == HAL_OK) ? STATUS_OK : STATUS_ERROR;
#else
    UNUSED(tx);
    UNUSED(rx);
    UNUSED(size);
    return STATUS_ERROR;
#endif
}
