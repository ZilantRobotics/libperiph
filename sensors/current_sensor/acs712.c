/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "acs712.h"

#define NUMBER_OF_ITERATIONS 3000

typedef struct {
    float zero_current;
    uint32_t calib_current_raw;
    uint32_t calib_iter;
    float adc_raw_to_dc_dc_out_current;
} Acs712Coeeficients;

static Acs712Coeeficients acs712 = {
    .zero_current = 1970,
    .calib_current_raw = 0,
    .calib_iter = 0,
    .adc_raw_to_dc_dc_out_current = 0.0216898f,
};


void acs712SetCoefficient(float new_adc_raw_to_dc_dc_out_current) {
    acs712.adc_raw_to_dc_dc_out_current = new_adc_raw_to_dc_dc_out_current;
}

float acs712ConvertToCurrent(uint16_t current_raw) {
    return (current_raw - acs712.zero_current) * acs712.adc_raw_to_dc_dc_out_current;
}

bool acs712PerformIterationOfCalibration(uint16_t current_raw) {
    if (acs712.calib_iter < NUMBER_OF_ITERATIONS) {
        acs712.calib_current_raw += current_raw;
        acs712.calib_iter++;
        return false;
    } else {
        acs712.zero_current = acs712.calib_current_raw / NUMBER_OF_ITERATIONS;
        acs712.calib_current_raw = 0;
        acs712.calib_iter = 0;
        return true;
    }
}

void acs712SetCalibratedValue(uint16_t current_raw) {
    if (current_raw <= 4095) {
        acs712.zero_current = current_raw;
    }
}
