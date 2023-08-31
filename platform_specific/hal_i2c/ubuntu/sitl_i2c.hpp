/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef PLATFORM_SPECIFIC_HAL_I2C_UBUNTU_SITL_H_
#define PLATFORM_SPECIFIC_HAL_I2C_UBUNTU_SITL_H_

#include <stdint.h>
#include <map>
#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

struct SitlI2CSensor {
    SitlI2CSensor(uint8_t id);

    virtual int8_t callback_on_i2c_transmit(uint8_t id, const uint8_t tx[], uint8_t len) = 0;
    virtual int8_t callback_on_i2c_receive(uint8_t id, uint8_t* rx, uint8_t len) = 0;

    uint8_t identifier;

    static constexpr const size_t MAX_NUMBER_OF_SENSOR = 5;
    static std::array<SitlI2CSensor*, MAX_NUMBER_OF_SENSOR> i2c_sensors;
    static size_t number_of_sensor;
};


#ifdef __cplusplus
}
#endif

#endif  // PLATFORM_SPECIFIC_HAL_I2C_UBUNTU_SITL_H_
