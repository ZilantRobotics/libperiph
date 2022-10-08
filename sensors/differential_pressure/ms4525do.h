/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file ms4525do.h
 * @author d.ponomarev
 */
#ifndef MS4525DO_H_
#define MS4525DO_H_

#include <stdint.h>

void ms4525doParse(float* raw_temperature, float* raw_diff_press);

/**
  * @brief  Only for test usage
  */
void ms4525doFillBuffer(const uint8_t new_buffer[]);

#endif  // MS4525DO_H_