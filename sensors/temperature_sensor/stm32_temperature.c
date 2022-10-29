/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file stm32_temperature.c
 * @author d.ponomarev
 * @brief Implementation of TemperatureSensor
 * @date Nov 18, 2018
 * @note stm32f103 datasheet and manual:
 * - Reference manual 11.10 Temperature sensor: https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
 * - Datasheet 5.3.19 Temperature sensor characteristics: https://www.st.com/resource/en/datasheet/stm32f103c8.pdf
 */

#include "stm32_temperature.h"

///< According to the datasheet:
#define AVG_SLOPE_MV_C              4.3
#define CELSIUS_25_V                1.43
#define CELSIUS_25_ADC_RAW          1774.5  //< 4095*1.43/3.3
#define CELSIUS_PER_ADC_RAW         5.3359  //< 4095*0.0043/3.3
#define CELSIUS_MINUS_40_ADC_RAW    2121.3  //< CELSIUS_25_ADC_RAW + (25 + 40) * CELSIUS_PER_ADC_RAW
#define CELSIUS_PLUS_125_ADC_RAW    1240.9  //< CELSIUS_25_ADC_RAW - 100 * CELSIUS_PER_ADC_RAW

uint16_t stm32TemperatureParse(uint16_t raw_temp)
{
    const uint16_t V25 = 1750;      // when V25=1.41V at ref 3.3V
    const uint16_t AVG_SLOPE = 5;   // when AVG_SLOPE=4.3mV/C at ref 3.3V
    uint16_t temperature = (V25 - raw_temp) / AVG_SLOPE + 25 + 273;
    return temperature;
}
