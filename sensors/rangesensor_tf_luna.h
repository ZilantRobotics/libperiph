/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file rangesensor_tf_luna.h
 * @author d.ponomarev
 * @note https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf
 */
#ifndef RANGESENSOR_TF_LUNA_H_
#define RANGESENSOR_TF_LUNA_H_

#include <stdbool.h>
#include <stdint.h>

#define TF_LUNA_SERIAL_FRAME_SIZE   9
#define TF_LUNA_BUFFER_SIZE         18


/**
 * @return
 */
bool tfLunaNextByte(uint8_t byte);

/**
 * @return
 */
bool tfLunaParseSerialFrame();

/**
 * @return range in meters
 */
float tfLunaGetRange();

#endif  // RANGESENSOR_TF_LUNA_H_
