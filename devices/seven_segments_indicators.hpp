/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file seven_segments_indicators.hpp
 * @author d.ponomarev
 * @date Jul 9, 2018
 */

#ifndef __SEVEN_SEGMENTS_INDICATORS_HPP
#define __SEVEN_SEGMENTS_INDICATORS_HPP

#include "stdint.h"

/**
* @brief Indicators
*/
class Indicators
{
	public:
		Indicators(): digit(0) {};
		void SetNumber(uint16_t number);
		void SetNumber(float number);
	private:
		uint8_t digit;
};


#endif	// __SEVEN_SEGMENTS_INDICATORS_HPP
