/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "i2c_manager.h"
#include <stdbool.h>
#include "config.h"
#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


#define I2C_MAX_BLOCK_TIME_MS   50
#define STATUS_WORKING          1
#define STATUS_IDLE             0

#ifndef NUM_OF_I2C_CLIENTS
    #define NUM_OF_I2C_CLIENTS 10
#endif

typedef struct {
    xSemaphoreHandle semaphore;
    StaticSemaphore_t sem_buffer;
    void (*function)();
    uint8_t status;
    uint8_t priority;
} I2C_Request_t;

///< According to the ISO, array size should be at least 1
#if NUM_OF_I2C_CLIENTS > 0
static I2C_Request_t requests[NUM_OF_I2C_CLIENTS] = {0};
#else
static I2C_Request_t requests[1] = {0};
#endif

static uint8_t sensors_amount = 0;


static bool i2cManagerIsComplete(uint8_t device_id);


int8_t i2cManagerRegister(uint8_t priority) {
    if (sensors_amount < NUM_OF_I2C_CLIENTS) {
        requests[sensors_amount].priority = priority;
        requests[sensors_amount].function = NULL;
        requests[sensors_amount].semaphore = xSemaphoreCreateCountingStatic(
                                                 1,
                                                 0,
                                                 &requests[sensors_amount].sem_buffer);
        if (requests[sensors_amount].semaphore == NULL) {
            return STATUS_ERROR;
        }
        return sensors_amount++;
    }
    return STATUS_ERROR;
}

int8_t i2cManagerPerformRequest(int8_t dev, void (*function)()) {
    if (dev >= 0 && dev < sensors_amount && function != NULL) {
        requests[dev].status = STATUS_WORKING;
        requests[dev].function = function;
        if (xSemaphoreTake(requests[dev].semaphore, I2C_MAX_BLOCK_TIME_MS) != pdTRUE) {
            return STATUS_ERROR;
        }
    }
    return (i2cManagerIsComplete(dev)) ? STATUS_OK : STATUS_ERROR;
}

void i2cManagerSpin() {
    for (uint8_t dev = 0; dev < sensors_amount; dev++) {
        if (!i2cManagerIsComplete(dev)) {
            (requests[dev].function)();
            requests[dev].status = STATUS_IDLE;
            xSemaphoreGive(requests[dev].semaphore);
        } else {
            continue;
        }
    }
}

bool i2cManagerIsComplete(uint8_t device_id) {
    return (requests[device_id].status == STATUS_WORKING) ? false : true;
}
