/*
 * Copyright (C) 2020-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file rm3100.h
 * @author d.ponomarev
 */
#ifndef SENSORS_MAGNETOMERE_RM3100_H_
#define SENSORS_MAGNETOMERE_RM3100_H_

#include <stdint.h>

#define RM3100_MEASUREMENT_FREQUENCY    75

int8_t rm3100Init();
int8_t rm3100Measure();
void rm310GetMeasurement(float* x, float* y, float* z);

#endif  // SENSORS_MAGNETOMERE_RM3100_H_
