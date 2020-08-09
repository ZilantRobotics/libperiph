/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file bmp280.c
 * @author d.ponomarev
 */

#include "bmp280.h"

#define I2C_ID                  0xEE
#define ID_REG                  0xD0
#define CTRL_MEAS_REG           0xF4
#define PRESS_REG               0xF7
#define CTRL_MEAS_NORMAL_MODE   3
