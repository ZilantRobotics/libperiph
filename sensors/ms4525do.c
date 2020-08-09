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
#include <stdint.h>


ms4525do_t airdata;

void getMS4525DOdata(void){
    int16_t dp_raw = 0, dT_raw = 0;
    dp_raw = (airdata.rx[0] << 8) + airdata.rx[1];
    dp_raw = 0x3FFF & dp_raw;
    dT_raw = (airdata.rx[2] << 8) + airdata.rx[3];
    dT_raw = (0xFFE0 & dT_raw) >> 5;
    float temperature = ((200.0f * dT_raw) / 2047) - 50;

    const float P_min = -1.0f;
    const float P_max = 1.0f;
    const float PSI_to_Pa = 6894.757f;

    float diff_press_PSI = -((dp_raw - 0.1f * 16383) * (P_max - P_min) / (0.8f * 16383) + P_min);
    float diff_press_pa_raw = diff_press_PSI * PSI_to_Pa;

    airdata.temperature = temperature;
    airdata.diff_press_pa_raw = diff_press_pa_raw;
}
