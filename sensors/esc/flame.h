/*
 * Copyright (C) 2018-2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file esc_flame.h
 * @author d.ponomarev
 * @date May 27, 2021
 */

#ifndef HMC5883L_H_
#define HMC5883L_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


#define ESC_FLAME_PACKAGE_SIZE 24


typedef struct {
    uint32_t error_count;           // uint32
    float voltage;                  // float16  Volt
    float current;                  // float16  Ampere
    float temperature;              // float16  Kelvin
    int32_t rpm;                    // int18
    uint8_t power_rating_pct;       // uint7 (range 0% to 127%)
    uint8_t esc_index;              // uint5
} EscFlameStatus_t;

/**
 * @note UART buffer size depends on the preferred strategy: memory or performance.
 * Minimal size is PACKAGE_SIZE + 1 requires calling memcpy every single package
 * Bigger size allows to call it less often.
 */
#define UART_BUFFER_SIZE 96
typedef struct {
    uint8_t buf[UART_BUFFER_SIZE];
    size_t prev_idx;
    size_t crnt_idx;
    size_t saved_idx;
} UartDmaParser_t;


void escFlameParseDma(uint8_t last_recv_idx, UartDmaParser_t* parser, EscFlameStatus_t* esc_status);


/**
  * @param[in] raw_package_buffer must be a buffer with size of a package
  * @return true if package is correct, otherwise false
  */
bool escFlameIsItPackageStart(const uint8_t* raw_package_buffer);

/**
  * @param[in] raw_package_buffer must be a correct package
  * @param[out] esc_status - struct with parsed package
  */
void escFlameParse(const uint8_t* raw_package_buffer, EscFlameStatus_t* esc_status);

#endif  // HMC5883L_H_
