/*
 * Copyright (C) 2020-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_DIFFERENTIAL_PRESSURE_MS4525DO_H_
#define SENSORS_DIFFERENTIAL_PRESSURE_MS4525DO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "libperiph_common.h"

typedef struct {
    float temperature;
    float diff_pressure;
} DifferentialPressureData;

/**
  * @brief Run it once. It returns LIBPERIPH_OK on success and < 0 on failure.
  */
int8_t ms4525doInit();


/**
  * @brief In a baremetal application you may want to call these functions one by one consequently.
  * Alternatively, for example in RTOS applications, you can call them separately.
  * ms4525doMeasure is blocking function and can take some time.
  * ms4525doMeasure just perform data serialization.
  */
void ms4525doMeasure();
DifferentialPressureData ms4525doParse();

/**
  * @brief Only for test usage
  */
void ms4525doFillBuffer(const uint8_t new_buffer[]);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_DIFFERENTIAL_PRESSURE_MS4525DO_H_
