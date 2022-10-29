/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ms4525do.c
 * @author d.ponomarev
 */

#include "ms4525do.h"
#include <string.h>
#include "libperiph_common.h"
#include "hal_i2c.h"


#define I2C_ID              (0x28 << 1) + 1
#define I2C_RESPONSE_SIZE   4

static uint8_t ms4525do_rx_buf[I2C_RESPONSE_SIZE] = {0x00};

int8_t ms4525doInit() {
    return STATUS_OK;
}

void ms4525doMeasure() {
    i2cReceive(I2C_ID, ms4525do_rx_buf, I2C_RESPONSE_SIZE);
}

void ms4525doParse(float* raw_temperature, float* raw_diff_press) {
    int16_t dp_raw = 0, dT_raw = 0;
    dp_raw = (ms4525do_rx_buf[0] << 8) + ms4525do_rx_buf[1];
    /* mask the used bits */
    dp_raw = 0x3FFF & dp_raw;
    dT_raw = (ms4525do_rx_buf[2] << 8) + ms4525do_rx_buf[3];
    dT_raw = (0xFFE0 & dT_raw) >> 5;
    float temperature = ((200.0f * dT_raw) / 2047) - 50;

    // Calculate differential pressure. As its centered around 8000
    // and can go positive or negative
    const float P_min = -1.0f;
    const float P_max = 1.0f;
    const float PSI_to_Pa = 6894.757f;
    /*
    this equation is an inversion of the equation in the
    pressure transfer function figure on page 4 of the datasheet
    We negate the result so that positive differential pressures
    are generated when the bottom port is used as the static
    port on the pitot and top port is used as the dynamic port
    */
    float diff_press_PSI = -((dp_raw - 0.1f * 16383) * (P_max - P_min) / (0.8f * 16383) + P_min);
    float diff_press_pa_raw = diff_press_PSI * PSI_to_Pa;

    *raw_temperature = temperature;
    *raw_diff_press = diff_press_pa_raw;
}

void ms4525doFillBuffer(const uint8_t new_buffer[]) {
    memcpy(ms4525do_rx_buf, new_buffer, I2C_RESPONSE_SIZE);
}
