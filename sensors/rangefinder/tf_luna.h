/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file tf_luna.h
 * @author d.ponomarev
 * @note https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf
 */
#ifndef RANGESENSOR_TF_LUNA_H_
#define RANGESENSOR_TF_LUNA_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define TF_LUNA_SERIAL_FRAME_SIZE   9
#define TF_LUNA_BUFFER_SIZE         18

/**
 * @return STATUS_OK if there is no error, otherwise STATUS_ERROR
 */
int8_t tfLunaInit();


/**
 * @brief Parse UART buffer
 * @return true if frame appear, otherwise false
 */
bool tfLunaCollectData(const uint8_t* buffer_ptr);

/**
 * @return range in meters
 */
float tfLunaParseCollectedData();

#endif  // RANGESENSOR_TF_LUNA_H_
