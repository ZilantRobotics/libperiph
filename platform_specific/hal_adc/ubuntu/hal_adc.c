/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_adc.h"
#include <stdbool.h>
#include <string.h>
#include "libperiph_common.h"

static uint8_t number_of_channels = 0;
static bool is_adc_already_inited = false;
uint16_t ubuntu_adc[MAX_CHANNELS_NUM] __attribute__((weak)) = {};

int8_t adcInitDma(uint8_t num_of_channels) {
    if (num_of_channels >= MAX_CHANNELS_NUM) {
        return LIBPERIPH_ERROR;
    }

    number_of_channels = num_of_channels;
    is_adc_already_inited = true;
    return LIBPERIPH_OK;
}

int8_t adcGetAll(uint16_t* adc_measurements) {
    if (adc_measurements == NULL) {
        return LIBPERIPH_ERROR;
    }

    memcpy((void*)adc_measurements, (void*)ubuntu_adc, number_of_channels * sizeof(uint16_t));
    return number_of_channels;
}

uint16_t adcGet(uint8_t rank) {
    return (rank >= number_of_channels) ? 65535 : ubuntu_adc[rank];
}

int8_t adcInitWithoutDma(uint8_t num_of_channels) {
    return STATUS_OK;
}

int8_t adcMeasureWithoutDma(uint16_t values[]) {
    return STATUS_OK;
}
