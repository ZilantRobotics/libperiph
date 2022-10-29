/*
 * Copyright (C) 2019-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file mpu9250.h
 * @author d.ponomarev
 */
#ifndef SENSORS_IMU_MPU9250_H_
#define SENSORS_IMU_MPU9250_H_

#include <stdbool.h>

bool mpu9250Init();
void mpu9250Measure();
void mpu9250GetMeasurements(float linear_acceleration[3], float angular_velocity[3]);

#endif  // SENSORS_IMU_MPU9250_H_
