/*
 * Copyright (C) 2020-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "sf1xx.h"
#include <string.h>
#include "main.h"
#include "hal_i2c.h"


#define PRODUCT_NAME_LENGTH     16
#define I2C_ID                  (0x66 << 1)
#define I2C_RESPONSE_SIZE       2


typedef enum {
    LW_PRODUCT_NAME = 0,
    LW_DISTANCE_OUTPUT = 27,
    LW_DISTANCE_DATA = 44,
    LW_WEASUREMENT_MODE = 93,
    LW_ZERO_OFFSET = 94,
    LW_LOST_SIGNAL_COUNTER = 95,
    LW_PROTOCOL = 120,
    LW_SERVO_CONNECTED = 121,
} LightwareRegister;

typedef struct {
    float min_distance;
    float max_distance;
} SensorProperties;


static uint8_t i2c_response_buf[I2C_RESPONSE_SIZE];
static const SensorProperties PROPERTIES[LW_KNOWN_SENSOR_TYPES_AMOUNT];


uint8_t lw_rangefinder_product_name[PRODUCT_NAME_LENGTH];


int8_t sf1xxInit() {
    i2cReadRegister(I2C_ID, LW_PRODUCT_NAME, lw_rangefinder_product_name, PRODUCT_NAME_LENGTH);
    return LIBPERIPH_OK;
}

int8_t sf1xxCollectData() {
    memset(i2c_response_buf, 0x00, I2C_RESPONSE_SIZE);
    return i2cReceive(I2C_ID, i2c_response_buf, I2C_RESPONSE_SIZE);
}

float sf1xxParseCollectedData() {
    return ((i2c_response_buf[0] << 8) + i2c_response_buf[1]) * 0.01f;
}

float lightwareGetMinDistance(LightwareSensor sensor_type) {
    if (sensor_type >= LW_KNOWN_SENSOR_TYPES_AMOUNT) {
        return 0.0f;
    }

    return PROPERTIES[sensor_type].min_distance;
}

float lightwareGetMaxDistance(LightwareSensor sensor_type) {
    if (sensor_type >= LW_KNOWN_SENSOR_TYPES_AMOUNT) {
        return 0.0f;
    }

    return PROPERTIES[sensor_type].max_distance;
}
