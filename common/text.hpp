/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file text.hpp
 * @author d.ponomarev
 * @date Jun 26, 2018
 */

#ifndef COMMON_TEXT_HPP
#define COMMON_TEXT_HPP

#include "stdint.h"

/**
* @brief Translation from num(int32_t) to string
* @param[in] intNum - integer number
* @param[in] str - �-string
* @note It may require no more than 12 bytes bytes (11 bytes - int32_t number, 1 byte - '\n')
*/
void num2str(int32_t num, uint8_t* str);

#endif  // COMMON_TEXT_HPP
