/*
 * Copyright (C) 2020-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef BAROMETER_BMP280_H_
#define BAROMETER_BMP280_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BMP280_MAX_MEASUREMENT_FREQUENCY 50

void bmp280Init();
bool bmp280IsInitialized();
void bmp280Calibrate();
void bmp280CollectData();
void bmp280ParseData();

float bmp280GetStaticPressure();
float bmp280GetStaticTemperature();

#ifdef __cplusplus
}
#endif

#endif  // BAROMETER_BMP280_H_
