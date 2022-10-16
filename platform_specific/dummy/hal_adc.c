/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_adc.h"

#ifndef STATUS_OK
    #define STATUS_OK 0
#endif

int8_t adcInitDma(uint8_t num_of_channels) {
    return STATUS_OK;
}

int8_t adcGetAll(uint16_t* adc_measurements) {
    return STATUS_OK;
}

uint16_t adcGet(uint8_t rank) {
    return STATUS_OK;
}

int8_t adcInitWithoutDma(uint8_t num_of_channels) {
    return STATUS_OK;
}

int8_t adcMeasureWithoutDma(uint16_t values[]) {
    return STATUS_OK;
}
