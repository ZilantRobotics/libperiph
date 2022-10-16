/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SRC_HAL_STM32_I2C_MANAGER_H_
#define SRC_HAL_STM32_I2C_MANAGER_H_

#include <stdint.h>
#include "hal_i2c.h"


/**
 * @brief Initialize an i2c master node by creating a semaphore
 * @return descriptor > 0 after success, otherwise STATUS_ERROR
 */
int8_t i2cManagerRegister(uint8_t priority);

/**
 * @brief Set a flag that a node is ready for performing request and
 * take a semaphore until this request is completed
 */
int8_t i2cManagerPerformRequest(int8_t device_id, void (*function)());

/**
 * @brief Spin all requests
 */
void i2cManagerSpin();


int8_t i2cWriteRegisterOneByte(uint8_t dev_id, uint8_t reg_addr, uint8_t new_reg_value);
int8_t i2cReadRegister(uint8_t dev_id, uint8_t reg_addr, uint8_t* out, uint8_t out_length);

#endif  // SRC_HAL_STM32_I2C_MANAGER_H_
