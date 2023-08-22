/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
    181, 98, 6, 0, 20, 0, 1, 0, 0, 0,
    192, 8, 0, 0, 0, 16, 14, 0, 35, 0,
    35, 0, 0, 0, 0, 0, 71, 250
};

static uint8_t ubxRates10Hz[] = {
    0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x7A, 0x12, 0xB5, 0x62, 0x06, 0x08, 0x00, 0x00,
    0x0E, 0x30
};

static uint8_t CfgNav5[] = {
    181, 98, 6, 36, 36, 0, 255, 5, 7, 3,
    0, 0, 0, 0, 16, 39, 0, 0, 10, 0,
    250, 0, 250, 0, 100, 0, 94, 1, 0, 60,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 144, 116
};
static uint8_t CfgTp5[] = {
    181, 98, 6, 49, 32, 0, 0, 1, 0, 0,
    50, 0, 0, 0, 160, 134, 1, 0, 160, 134,
    1, 0, 16, 39, 0, 0, 16, 39, 0, 0,
    0, 0, 0, 0, 119, 0, 0, 0, 189, 152
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
    181, 98, 6, 1, 8, 0, 13, 3, 0, 1, 0, 0, 0, 0, 32, 37
};
static uint8_t ubxSaveConfig[] = {
    181, 98, 6, 9, 13, 0, 0, 0, 0, 0, 31, 31, 0, 0, 0, 0, 0, 0, 3, 93, 203
};

static uint8_t ubxDisableGGA[] = {181, 98, 6, 1, 8, 0, 240, 0, 0, 0, 0, 0, 0, 0, 255, 35};
static uint8_t ubxDisableGLL[] = {181, 98, 6, 1, 8, 0, 240, 1, 0, 0, 0, 0, 0, 0, 0, 42};
static uint8_t ubxDisableGSA[] = {181, 98, 6, 1, 8, 0, 240, 2, 0, 0, 0, 0, 0, 0, 1, 49};
static uint8_t ubxDisableGSV[] = {181, 98, 6, 1, 8, 0, 240, 3, 0, 0, 0, 0, 0, 0, 2, 56};
static uint8_t ubxDisableRMC[] = {181, 98, 6, 1, 8, 0, 240, 4, 0, 0, 0, 0, 0, 0, 3, 63};
static uint8_t ubxDisableVTG[] = {181, 98, 6, 1, 8, 0, 240, 5, 0, 0, 0, 0, 0, 0, 4, 70};

static UbloxCommand ubxConfigurationSequence[] = {
    NODE_SET_BAUDRATE_9600,
    UBX_CMD_FACTORY_RESET,
    UBX_CMD_BAUDRATE_921600,

    NODE_SET_BAUDRATE_38400,
    UBX_CMD_FACTORY_RESET,
    UBX_CMD_BAUDRATE_921600,

    NODE_SET_BAUDRATE_115200,
    UBX_CMD_FACTORY_RESET,
    UBX_CMD_BAUDRATE_921600,

    NODE_SET_BAUDRATE_921600,
    UBX_CMD_FACTORY_RESET,
    UBX_CMD_BAUDRATE_921600,

    UBX_CFG_NAV_5,
    UBX_CFG_TP_5,

    UBX_CMD_DISABLE_GGA,
    UBX_CMD_DISABLE_GLL,
    UBX_CMD_DISABLE_GSA,
    UBX_CMD_DISABLE_GSV,
    UBX_CMD_DISABLE_RMC,
    UBX_CMD_DISABLE_VTG,

    UBX_MON_HW,
    UBX_NAV_COV,
    UBX_NAV_PVT,
    UBX_NAV_STATUS,
    UBX_TIM_TIM2,

    UBX_CMD_RATE_10_HZ,
    UBX_CMD_SAVE_CONFIG,
};
#define UBLOX_COMMAND_AMOUNT (sizeof(ubxConfigurationSequence) / sizeof(UbloxCommand))
static_assert(UBLOX_COMMAND_AMOUNT == 27, "Wrong size");

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

        case NODE_SET_BAUDRATE_9600:
            ubxChangeBaudRate(9600);
            result = 0;
            break;
        case NODE_SET_BAUDRATE_38400:
            ubxChangeBaudRate(38400);
            result = 0;
            break;
        case NODE_SET_BAUDRATE_115200:
            ubxChangeBaudRate(115200);
            result = 0;
            break;
        case NODE_SET_BAUDRATE_921600:
            ubxChangeBaudRate(921600);
            result = 0;
            break;

        case UBX_CFG_NAV_5:
            result = ubxTransmit(CfgNav5, sizeof(CfgNav5));
            break;
        case UBX_CFG_TP_5:
            result = ubxTransmit(CfgTp5, sizeof(CfgTp5));
            break;

        case UBX_CMD_DISABLE_GGA:
            result = ubxTransmit(ubxDisableGGA, sizeof(ubxDisableGGA));
            break;
        case UBX_CMD_DISABLE_GLL:
            result = ubxTransmit(ubxDisableGLL, sizeof(ubxDisableGLL));
            break;
        case UBX_CMD_DISABLE_GSA:
            result = ubxTransmit(ubxDisableGSA, sizeof(ubxDisableGSA));
            break;
        case UBX_CMD_DISABLE_GSV:
            result = ubxTransmit(ubxDisableGSV, sizeof(ubxDisableGSV));
            break;
        case UBX_CMD_DISABLE_RMC:
            result = ubxTransmit(ubxDisableRMC, sizeof(ubxDisableRMC));
            break;
        case UBX_CMD_DISABLE_VTG:
            result = ubxTransmit(ubxDisableVTG, sizeof(ubxDisableVTG));
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

UbloxCommand ubloxGetCommand(uint8_t idx) {
    if (idx >= UBLOX_COMMAND_AMOUNT) {
        return UBX_UNKNOWN_COMMAND;
    }

    return ubxConfigurationSequence[idx];
}

int8_t ubloxConfigure(uint16_t delay) {
    int8_t result = 0;

    for (uint_fast8_t cmd_idx = 0; cmd_idx < UBLOX_COMMAND_AMOUNT; cmd_idx++) {
        result |= ubloxExecuteCommand(ubxConfigurationSequence[cmd_idx]);
        ubxDelay(delay);
    }

    return result;
}
