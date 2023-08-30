/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef MS4525DO_PARSER_H_
#define MS4525DO_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "libperiph_common.h"

int8_t ms4525doInit();
void ms4525doMeasure();
void ms4525doParse(float* raw_temperature, float* raw_diff_press);

/**
  * @brief  Only for test usage
  */
void ms4525doFillBuffer(const uint8_t new_buffer[]);

#ifdef __cplusplus
}
#endif

#endif  // MS4525DO_PARSER_H_
