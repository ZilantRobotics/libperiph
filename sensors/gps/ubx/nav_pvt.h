/*
 * Copyright (C) 2019-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdint.h>
#include <assert.h>

typedef struct {
    uint32_t time_of_week_ms;   ///< iTOW Timestamp
    uint16_t year_utc;
    uint8_t month_utc;
    uint8_t day_utc;
    uint8_t hour_utc;
    uint8_t min_utc;
    uint8_t sec_utc;
    uint8_t valid;
    uint32_t tAcc;              ///< Time accuracy estimate (UTC), ns
    int32_t nano;               ///< Fraction of second, range -1e9 .. 1e9 (UTC)
    uint8_t fixType;
    uint8_t flags;
    uint8_t flags2;
    uint8_t numSV;              ///< Number of satellites used in Nav Solution
    int32_t lon;                ///< deg, 1e-7
    int32_t lat;                ///< deg, 1e-7
    int32_t height;             ///< Height above ellipsoid, mm
    int32_t hMSL;               ///< Height above mean sea level, mm
    uint32_t gAcc;              ///< Horizontal accuracy estimate, mm
    uint32_t vAcc;              ///< Vertical accuracy estimate, mm
    int32_t velN;               ///< mm/s
    int32_t velE;               ///< mm/s
    int32_t velD;               ///< mm/s
    int32_t gSpeed;
    int32_t headMot;
    uint32_t sAcc;
    uint32_t headAcc;
    uint16_t pDOP;
    uint8_t flags3;
    uint8_t reserved1[5];
    int32_t headVeh;
    int16_t magDec;
    uint16_t magAcc;
} UbxNavPvt_t;
static_assert(sizeof(UbxNavPvt_t) == 92, "Wrong size");

#pragma pack(push, 1)
typedef struct {
    uint8_t sync_char_1;
    uint8_t sync_char_2;
    uint8_t class_nav;
    uint8_t id_nav_pvt;
    uint16_t payload_length;
    UbxNavPvt_t payload;
    uint16_t crc;
} UbxNavPvtRaw_t;
#pragma pack(pop)
