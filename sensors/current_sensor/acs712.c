/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file current_sensor/acs712.c
 * @author d.ponomarev
 */

#include "acs712.h"


///< Calibration
static float zero_current = 1970;
static uint32_t calib_current_raw = 0;
static uint32_t calib_iter = 0;
static const uint32_t NUMBER_OF_ITERATIONS = 3000;

static float adc_raw_to_dc_dc_out_current = 0.0216898;

void acs712SetCoefficient(float new_adc_raw_to_dc_dc_out_current) {
    adc_raw_to_dc_dc_out_current = new_adc_raw_to_dc_dc_out_current;
}

float acs712ConvertToCurrent(uint16_t current_raw) {
    return (current_raw - zero_current) * adc_raw_to_dc_dc_out_current;
}

bool acs712PerformIterationOfCalibration(uint16_t current_raw) {
    if (calib_iter < NUMBER_OF_ITERATIONS) {
        calib_current_raw += current_raw;
        calib_iter++;
        return false;
    } else {
        zero_current = calib_current_raw / NUMBER_OF_ITERATIONS;
        calib_current_raw = 0;
        calib_iter = 0;
        return true;
    }
}

void acs712SetCalibratedValue(uint16_t current_raw) {
    if (current_raw <= 4095) {
        zero_current = current_raw;
    }
}
