/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file target.hpp
 * @author d.ponomarev
 * @date Jun 23, 2018
 */

#ifndef __TARGET_HPP
#define __TARGET_HPP

#include "stm32f3xx.h"

enum
{
	SYSCLOCK = 8000000,	///< 8 MHz - system clock frequency
};

/**
* @brief Board driver
*/
class Target
{
	public:
		enum Led_t
		{
			LD4 =  8,
			LD3 =  9,	///< North
			LD5 =  10,
			LD7 =  11,	///< East
			LD9 =  12,
			LD10 = 13,	///< South
			LD8 =  14,
			LD6 =  15,	///< West
			LD_MIN = 8,
			LD_MAX = 15,
		};
		static void InitGPIO();
		static void LedOn(uint8_t ledNumber);
		static void LedOff(uint8_t ledNumber);
};


#endif //__TARGET_HPP
