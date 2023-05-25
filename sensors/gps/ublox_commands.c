/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ublox_commands.c
 * @author ehsan shaghaei
 * @date Dec 21, 2022
 */

#include "ublox_commands.h"
#include <assert.h>
#include "libperiph_common.h"


static UbxTransmit_t ubxTransmit = NULL;
static UbxDelay_t ubxDelay = NULL;
static UbxChangeBaudRate_t ubxChangeBaudRate = NULL;


static uint8_t uBloxConfigFactoryReset[] = {
    0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x1B,
    0x9A
};

static uint8_t ubxBaudRate921600[] = {
    181, 98, 6, 0, 20, 0, 1, 0, 0, 0, 192, 8, 0, 0, 0, 16, 14, 0, 35, 0, 35, 0, 0, 0, 0, 0, 71, 250
};

static uint8_t ubxRates10Hz[] = {
    0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x7A, 0x12, 0xB5, 0x62, 0x06, 0x08, 0x00, 0x00,
    0x0E, 0x30
};

static uint8_t ubxMonHw[] = {
    181, 98, 6, 1, 8, 0, 10, 9, 0, 1, 0, 0, 0, 0, 35, 55
};
static uint8_t ubxNavCov[] = {
    181, 98, 6, 1, 8, 0, 1, 54, 0, 1, 0, 0, 0, 0, 71, 42
};
static uint8_t ubxNavPvt[] = {
    181, 98, 6, 1, 8, 0, 1, 7, 0, 1, 0, 0, 0, 0, 24, 225
};
static uint8_t ubxNavStatus[] = {
    181, 98, 6, 1, 8, 0, 1, 3, 0, 1, 0, 0, 0, 0, 20, 197
};
static uint8_t ubxTimTim2[] = {
    181,98,6,1,8,0,13,3,0,1,0,0,0,0,32,37
};
static uint8_t ubxSaveConfig[] = {
    181, 98, 6, 9, 13, 0, 0, 0, 0, 0, 31, 31, 0, 0, 0, 0, 0, 0, 3, 93, 203
};

static UbloxCommand ubxConfigurationSequence[] = {
    UBX_SET_BAUDRATE_9600,
    UBX_CMD_FACTORY_RESET,
    UBX_CMD_BAUDRATE_921600,

    UBX_SET_BAUDRATE_115200,
    UBX_CMD_FACTORY_RESET,
    UBX_CMD_BAUDRATE_921600,

    UBX_SET_BAUDRATE_921600,
    UBX_CMD_FACTORY_RESET,
    UBX_CMD_BAUDRATE_921600,

    UBX_MON_HW,
    UBX_NAV_COV,
    UBX_NAV_PVT,
    UBX_NAV_STATUS,
    UBX_TIM_TIM2,

    UBX_CMD_RATE_10_HZ,
    UBX_CMD_SAVE_CONFIG,
};
static_assert(sizeof(ubxConfigurationSequence) == 16);

int8_t ubloxInit(UbxTransmit_t transmit, UbxDelay_t delay, UbxChangeBaudRate_t changeBaudRate) {
    if (transmit == NULL || delay == NULL || changeBaudRate == NULL) {
        return -1;
    }

    ubxTransmit = transmit;
    ubxDelay = delay;
    ubxChangeBaudRate = changeBaudRate;

    return 0;
}

int8_t ubloxExecuteCommand(UbloxCommand command) {
    if (ubxTransmit == NULL) {
        return -1;
    }

    int8_t result;
    switch (command) {
        case UBX_CMD_FACTORY_RESET:
            result = ubxTransmit(uBloxConfigFactoryReset, sizeof(uBloxConfigFactoryReset));
            break;

        case UBX_CMD_BAUDRATE_921600:
            result = ubxTransmit(ubxBaudRate921600, sizeof(ubxBaudRate921600));
            break;

        case UBX_SET_BAUDRATE_9600:
            ubxChangeBaudRate(9600);
            result = 0;
            break;
        case UBX_SET_BAUDRATE_115200:
            ubxChangeBaudRate(115200);
            result = 0;
            break;
        case UBX_SET_BAUDRATE_921600:
            ubxChangeBaudRate(921600);
            result = 0;
            break;

        case UBX_MON_HW:
            result = ubxTransmit(ubxMonHw, sizeof(ubxMonHw));
            break;

        case UBX_NAV_COV:
            result = ubxTransmit(ubxNavCov, sizeof(ubxNavCov));
            break;

        case UBX_NAV_PVT:
            result = ubxTransmit(ubxNavPvt, sizeof(ubxNavPvt));
            break;

        case UBX_NAV_STATUS:
            result = ubxTransmit(ubxNavStatus, sizeof(ubxNavStatus));
            break;

        case UBX_TIM_TIM2:
            result = ubxTransmit(ubxTimTim2, sizeof(ubxTimTim2));
            break;

        case UBX_CMD_RATE_10_HZ:
            result = ubxTransmit(ubxRates10Hz, sizeof(ubxRates10Hz));
            break;

        case UBX_CMD_SAVE_CONFIG:
            result = ubxTransmit(ubxSaveConfig, sizeof(ubxSaveConfig));
            break;

        default:
            result = -1;
            break;
    }

    return result;
}

int8_t ubloxConfigure() {
    int8_t result = 0;

    for (uint_fast8_t cmd_idx = 0; cmd_idx < sizeof(ubxConfigurationSequence); cmd_idx++) {
        result |= ubloxExecuteCommand(ubxConfigurationSequence[cmd_idx]);
        ubxDelay(100);
    }

    return result;
}
