/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file encoder.hpp
 * @author d.ponomarev
 * @date Oct 20, 2018
 */

#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include <stdint.h>

enum EncoderPin
{
    LEFT_ENC_A_CH,
    LEFT_ENC_B_CH,
    RIGHT_ENC_A_CH,
    RIGHT_ENC_B_CH,
};

/**
* @brief Encoder
* @details There are few requirements and nuance:
* For incrementing and decrementing encoders values we use:
* - EXT (external interrupts with callbacks)
* For calculating encoders speed (number of impulses which we received for last
* 0.1 second) we use:
* We use following GPIO:
* - PE_10  - left encoder A
* - PE_12  - left encoder B
* - PE_14  - right encoder A
* - PE_15  - right encoder B
*/
class Encoder
{
public:
    static void Reset();
    static int32_t GetLeftValue();
    static int32_t GetRightValue();
    static int32_t GetLeftSpeed();
    static int32_t GetRightSpeed();
};

#endif /* ENCODER_HPP */
