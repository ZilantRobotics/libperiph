/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file hmc5883l.c
 * @author d.ponomarev
 * @date Nov 18, 2018
 */

#include "hmc5883l.h"

void hmc5883l_parse(const uint8_t buf[6], float mag[3]) {
	int16_t raw_data[3];
	raw_data[0] = (buf[0]<<8) + buf[1];
	raw_data[1] = (buf[2] << 8) + buf[3];
	raw_data[2] = (buf[4] << 8) + buf[5];

	mag[0] = raw_data[0]/1090.0;
	mag[1] = raw_data[1]/1090.0;
	mag[2] = raw_data[2]/1090.0;
}
