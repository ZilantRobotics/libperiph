/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_adc.h"
#include "libperiph_common.h"


int8_t adcInitDma(uint8_t num_of_channels) {
    return STATUS_OK;
}

int8_t adcGetAll(uint16_t* adc_measurements) {
    return STATUS_OK;
}

uint16_t adcGet(uint8_t rank) {
    uint16_t adc_value;
    switch (rank) {
        case 0:
            adc_value = 5.0 / (3.3 * 17.0 / 4095.0);
            break;

        case 1:
            adc_value = 5.0 / (3.3 * 2.0 / 4095.0);
            break;

        case 2:
            adc_value = 400;
            break;

        default:
            adc_value = 1750;
            break;
    }
    return adc_value;
}

int8_t adcInitWithoutDma(uint8_t num_of_channels) {
    return STATUS_OK;
}

int8_t adcMeasureWithoutDma(uint16_t values[]) {
    return STATUS_OK;
}
