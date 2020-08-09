/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file ms4525do.h
 * @author d.ponomarev
 */
#ifndef MS4525DO_H_
#define MS4525DO_H_

#define MS4525DO_ID 0x28<<1 + 1

typedef struct
{
	uint8_t rx[4];
	float temperature;
	float diff_press_pa_raw;
} ms4525do_t;

#endif  // MS4525DO_H_