/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file vl53l0x.c
 * @author d.ponomarev
 */

#include "vl53l0x.h"
#include <string.h>
#include "main.h"
#include "i2c_manager.h"
#include "params.h"

#define I2C_ID                                          (0x52)

#define VL53L0X_OUT_OF_RANGE                            (8190)

#define REG_IDENTIFICATION_MODEL_ID                     (0xC0)
#define REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV            (0x89)
#define REG_MSRC_CONFIG_CONTROL                         (0x60)
#define REG_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT (0x44)
#define REG_SYSTEM_SEQUENCE_CONFIG                      (0x01)
#define REG_DYNAMIC_SPAD_REF_EN_START_OFFSET            (0x4F)
#define REG_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD         (0x4E)
#define REG_GLOBAL_CONFIG_REF_EN_START_SELECT           (0xB6)
#define REG_SYSTEM_INTERRUPT_CONFIG_GPIO                (0x0A)
#define REG_GPIO_HV_MUX_ACTIVE_HIGH                     (0x84)
#define REG_SYSTEM_INTERRUPT_CLEAR                      (0x0B)
#define REG_RESULT_INTERRUPT_STATUS                     (0x13)
#define REG_SYSRANGE_START                              (0x00)
#define REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0            (0xB0)
#define REG_RESULT_RANGE_STATUS                         (0x14)

#define RANGE_SEQUENCE_STEP_DSS                         (0x28)
#define RANGE_SEQUENCE_STEP_PRE_RANGE                   (0x40)
#define RANGE_SEQUENCE_STEP_FINAL_RANGE                 (0x80)

typedef enum {
    CALIBRATION_TYPE_VHV,
    CALIBRATION_TYPE_PHASE
} Calibration_type_t;

static int8_t i2c_manager_id = STATUS_ERROR;
static uint16_t range = 0;
static uint8_t stop_variable = 0;

static bool i2c_write_addr8_data8(uint8_t reg, uint8_t value);
static bool i2c_read_addr8_data8(uint8_t reg, uint8_t* value);
static bool i2c_read_addr8_data16(uint8_t reg, uint16_t* value);

static bool vl53l0xConfigureInterrupt();
static bool vl53l0xDataInit();
static bool vl53l0xStaticInit();
static bool vl53l0xPerformRefCalibration();
static void vl53l0xMeasureCallback();
static bool vl53l0xProcessSingleMeasurement();
static bool vl53l0xSetSequenceStepsEnabled(uint8_t sequence_step);
static bool vl53l0xPerformSingleRefCalibration(Calibration_type_t calib_type);


int8_t vl53l0xInit(int8_t new_i2c_manager_id) {
    i2c_manager_id = new_i2c_manager_id;
    vl53l0xConfigureInterrupt();

    if (!vl53l0xDataInit()) {
        return false;
    }

    if (!vl53l0xStaticInit()) {
        return STATUS_ERROR;
    }

    if (!vl53l0xPerformRefCalibration()) {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}


bool vl53l0xCollectData(uint32_t measurement_period) {
    static uint32_t next_measurement_time_ms = 0;
    uint32_t crnt_time = HAL_GetTick();

    if (i2c_manager_id == STATUS_ERROR || crnt_time < next_measurement_time_ms) {
        return false;
    }
    next_measurement_time_ms = crnt_time + measurement_period;

    if (i2cManagerPerformRequest(i2c_manager_id, &vl53l0xMeasureCallback) == STATUS_ERROR) {
        return false;
    }

    return true;
}

float vl53l0xParseCollectedData() {
    if (range > 2000) {
        return 0.0;
    } else {
        return range * 0.001;
    }
    return range;
}

bool i2c_write_addr8_data8(uint8_t reg, uint8_t value) {
    return 0 == i2cWriteRegisterOneByte(I2C_ID, reg, value);
}
bool i2c_read_addr8_data8(uint8_t reg, uint8_t* value) {
    return 0 == i2cReadRegister(I2C_ID, reg, value, 1);
}
bool i2c_read_addr8_data16(uint8_t reg, uint16_t* value) {
    uint8_t buf[2] = {};
    int8_t res = i2cReadRegister(I2C_ID, reg, (uint8_t*)buf, 2);
    *value = (uint16_t)buf[1] + ((uint16_t)buf[0] << 8);
    return 0 == res;
}

bool vl53l0xSetSequenceStepsEnabled(uint8_t sequence_step) {
    return i2c_write_addr8_data8(REG_SYSTEM_SEQUENCE_CONFIG, sequence_step);
}

bool vl53l0xConfigureInterrupt() {
    /* Interrupt on new sample ready */
    if (!i2c_write_addr8_data8(REG_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x04)) {
        return false;
    }

    /* Configure active low since the pin is pulled-up on most breakout boards */
    uint8_t gpio_hv_mux_active_high = 0;
    if (!i2c_read_addr8_data8(REG_GPIO_HV_MUX_ACTIVE_HIGH, &gpio_hv_mux_active_high)) {
        return false;
    }
    gpio_hv_mux_active_high &= ~0x10;
    if (!i2c_write_addr8_data8(REG_GPIO_HV_MUX_ACTIVE_HIGH, gpio_hv_mux_active_high)) {
        return false;
    }

    if (!i2c_write_addr8_data8(REG_SYSTEM_INTERRUPT_CLEAR, 0x01)) {
        return false;
    }
    return true;
}

bool vl53l0xPerformSingleRefCalibration(Calibration_type_t calib_type) {
    uint8_t sysrange_start = 0;
    uint8_t sequence_config = 0;

    switch (calib_type) {
        case CALIBRATION_TYPE_VHV:
            sequence_config = 0x01;
            sysrange_start = 0x01 | 0x40;
            break;
        case CALIBRATION_TYPE_PHASE:
            sequence_config = 0x02;
            sysrange_start = 0x01 | 0x00;
            break;
    }

    if (!i2c_write_addr8_data8(REG_SYSTEM_SEQUENCE_CONFIG, sequence_config)) {
        return false;
    }

    if (!i2c_write_addr8_data8(REG_SYSRANGE_START, sysrange_start)) {
        return false;
    }

    /* Wait for interrupt */
    uint8_t interrupt_status = 0;
    bool success = false;

    uint_fast16_t attempt;
    for (attempt = 0; attempt <= 5000; attempt++) {
        success = i2c_read_addr8_data8(REG_RESULT_INTERRUPT_STATUS, &interrupt_status);
        if (!(success && ((interrupt_status & 0x07) == 0))) {
            break;
        }
    }
    if (attempt > 5000) {
        return false;
    }

    if (!success) {
        return false;
    }

    if (!i2c_write_addr8_data8(REG_SYSTEM_INTERRUPT_CLEAR, 0x01)) {
        return false;
    }

    if (!i2c_write_addr8_data8(REG_SYSRANGE_START, 0x00)) {
        return false;
    }

    return true;
}

bool vl53l0xPerformRefCalibration() {
    if (!vl53l0xPerformSingleRefCalibration(CALIBRATION_TYPE_VHV)) {
        return false;
    }

    if (!vl53l0xPerformSingleRefCalibration(CALIBRATION_TYPE_PHASE)) {
        return false;
    }

    if (!vl53l0xSetSequenceStepsEnabled(RANGE_SEQUENCE_STEP_DSS +
                                        RANGE_SEQUENCE_STEP_PRE_RANGE +
                                        RANGE_SEQUENCE_STEP_FINAL_RANGE)) {
        return false;
    }

    return true;
}

bool vl53l0xStaticInit() {
    if (!vl53l0xConfigureInterrupt()) {
        return false;
    }

    if (!vl53l0xSetSequenceStepsEnabled(RANGE_SEQUENCE_STEP_DSS +
                                        RANGE_SEQUENCE_STEP_PRE_RANGE +
                                        RANGE_SEQUENCE_STEP_FINAL_RANGE)) {
        return false;
    }

    return true;
}

bool vl53l0xDataInit() {
    /* Set 2v8 mode */
    uint8_t vhv_config_scl_sda = 0;
    if (!i2c_read_addr8_data8(REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, &vhv_config_scl_sda)) {
        return false;
    }
    vhv_config_scl_sda |= 0x01;
    if (!i2c_write_addr8_data8(REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, vhv_config_scl_sda)) {
        return false;
    }

    /* Set I2C standard mode */
    bool success = i2c_write_addr8_data8(0x88, 0x00);

    success &= i2c_write_addr8_data8(0x80, 0x01);
    success &= i2c_write_addr8_data8(0xFF, 0x01);
    success &= i2c_write_addr8_data8(0x00, 0x00);
    success &= i2c_read_addr8_data8(0x91, &stop_variable);
    success &= i2c_write_addr8_data8(0x00, 0x01);
    success &= i2c_write_addr8_data8(0xFF, 0x00);
    success &= i2c_write_addr8_data8(0x80, 0x00);

    return success;
}

bool vl53l0xRequestMeasurement() {
    if (!i2c_write_addr8_data8(REG_SYSTEM_INTERRUPT_CLEAR, 0x01)) {
        return false;
    }

    bool success = i2c_write_addr8_data8(0x80, 0x01);
    success &= i2c_write_addr8_data8(0xFF, 0x01);
    success &= i2c_write_addr8_data8(0x00, 0x00);
    success &= i2c_write_addr8_data8(0x91, stop_variable);
    success &= i2c_write_addr8_data8(0x00, 0x01);
    success &= i2c_write_addr8_data8(0xFF, 0x00);
    success &= i2c_write_addr8_data8(0x80, 0x00);

    if (!success) {
        return false;
    }

    if (!i2c_write_addr8_data8(REG_SYSRANGE_START, 0x01)) {
        return false;
    }

    return true;
}

bool vl53l0xWaitForResult(uint16_t max_tries) {
    uint8_t sysrange_start = 0;
    uint_fast16_t attempt;
    bool success = false;
    for (attempt = 0; attempt <= max_tries; attempt++) {
        success = i2c_read_addr8_data8(REG_SYSRANGE_START, &sysrange_start);
        if (!(success && (sysrange_start & 0x01))) {
            break;
        }
    }

    if (attempt > max_tries) {
        return false;
    }

    if (!success) {
        return false;
    }

    uint8_t interrupt_status = 0;

    for (attempt = 0; attempt <= max_tries; attempt++) {
        success = i2c_read_addr8_data8(REG_RESULT_INTERRUPT_STATUS, &interrupt_status);
        if (!(success && ((interrupt_status & 0x07) == 0))) {
            break;
        }
    }

    if (attempt > max_tries) {
        return false;
    }

    if (!success) {
        return false;
    }

    return true;
}

bool vl53l0xReadMeasurement() {
    if (!i2c_read_addr8_data16(REG_RESULT_RANGE_STATUS + 10, &range)) {
        return false;
    }

    if (range == 8190 || range == 8191) {
        range = VL53L0X_OUT_OF_RANGE;
    }

    return true;
}

bool vl53l0xProcessSingleMeasurement() {
    vl53l0xWaitForResult(100);
    vl53l0xReadMeasurement();
    return vl53l0xRequestMeasurement();
}


void vl53l0xMeasureCallback() {
    range = 0;

    uint16_t uid = 0;
    i2c_read_addr8_data16(REG_IDENTIFICATION_MODEL_ID, &uid);
    if ((uid >> 8) != 0xEE) {
        return;
    }

    vl53l0xProcessSingleMeasurement();
}
