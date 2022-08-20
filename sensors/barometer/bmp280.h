/*
 * Copyright (C) 2020-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file bmp280.h
 * @author d.ponomarev
 */

#ifndef BMP280_H_
#define BMP280_H_

#define BMP280_MAX_MEASUREMENT_FREQUENCY 50

void bmp280Init();
void bmp280Calibrate();
void bmp280CollectData();
void bmp280ParseData();

float bmp280GetStaticPressure();
float bmp280GetStaticTemperature();

#endif  // BMP280_H_
