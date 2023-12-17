/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_i2c.h"
#include <string.h>
#include "sitl_i2c.hpp"
#include "libperiph_common.h"


int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len) {
    if (tx == NULL || len == 0) {
        return LIBPERIPH_ERROR;
    }

    bool is_transmited = false;
    for (size_t idx = 0; idx < SitlI2CSensor::number_of_sensor; idx++) {
        if (SitlI2CSensor::i2c_sensors[idx]->callback_on_i2c_transmit(id, tx, len) >= 0) {
            is_transmited = true;
        }
    }

    return is_transmited ? LIBPERIPH_OK : LIBPERIPH_ERROR;
}

int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len) {
    if (rx == NULL || len == 0) {
        return LIBPERIPH_ERROR;
    }

    bool is_received = false;
    for (size_t idx = 0; idx < SitlI2CSensor::number_of_sensor; idx++) {
        if (SitlI2CSensor::i2c_sensors[idx]->callback_on_i2c_receive(id, rx, len) >= 0) {
            is_received = true;
        }
    }

    return is_received ? LIBPERIPH_OK : LIBPERIPH_ERROR;
}

std::array<SitlI2CSensor*, SitlI2CSensor::MAX_NUMBER_OF_SENSOR> SitlI2CSensor::i2c_sensors;
size_t SitlI2CSensor::number_of_sensor;
SitlI2CSensor::SitlI2CSensor(uint8_t id) : identifier(id) {
    if (number_of_sensor < MAX_NUMBER_OF_SENSOR) {
        i2c_sensors[number_of_sensor] = this;
        number_of_sensor++;
    }
}
