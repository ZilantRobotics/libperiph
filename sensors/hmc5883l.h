/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file hmc5883l.h
 * @author d.ponomarev
 * @date Nov 18, 2018
 */

#ifndef INC_HMC5883L_H_
#define INC_HMC5883L_H_

#include <stdint.h>

void hmc5883l_parse(const uint8_t buf[6], float mag[3]);

#endif  // INC_HMC5883L_H_
