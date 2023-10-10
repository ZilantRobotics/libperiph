/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_ESC_FLAME_H_
#define SENSORS_ESC_FLAME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"


#define ESC_FLAME_PACKAGE_SIZE 24


typedef struct {
    uint32_t counter;               // uint32
    float voltage;                  // float16  Volt
    float current;                  // float16  Ampere
    float temperature;              // float16  Kelvin
    int32_t rpm;                    // int18
    uint8_t power_rating_pct;       // uint7 (range 0% to 127%)
    uint8_t esc_index;              // uint5
} EscFlame_t;


void escSetAlphaParameters();
void escSetFlameParameters();

bool escFlameParseDma(size_t last_recv_idx, DmaUartHandler_t* parser, EscFlame_t* esc_status);
void escMotorNumberOfPoles(uint8_t mot_num_poles);

#ifdef LIBPERIPH_UNIT_TESTS
bool escFlameIsItPackageStart(const uint8_t* buffer);
void escFlameParse(const uint8_t* buffer, EscFlame_t* esc_status);
#endif


#ifdef __cplusplus
}
#endif

#endif  // SENSORS_ESC_FLAME_H_
