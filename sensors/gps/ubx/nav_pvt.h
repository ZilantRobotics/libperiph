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
    uint32_t time_of_week_ms;   ///< GPS Time of Week [ms]
    uint16_t year_utc;          ///< Year (UTC)
    uint8_t month_utc;          ///< Month, range 1..12 (UTC)
    uint8_t day_utc;            ///< Day of month, range 1..31 (UTC)
    uint8_t hour_utc;           ///< Hour of day, range 0..23 (UTC)
    uint8_t min_utc;            ///< Minute of hour, range 0..59 (UTC)
    uint8_t sec_utc;            ///< Seconds of minute, range 0..60 (UTC)
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
    int32_t velN;               ///< NED north velocity [mm/s]
    int32_t velE;               ///< NED east velocity [mm/s]
    int32_t velD;               ///< NED down velocity [mm/s]
    int32_t gSpeed;             ///< Ground Speed (2-D) [mm/s]
    int32_t headMot;
    uint32_t sAcc;              ///< Speed accuracy estimate [mm/s]
    uint32_t headAcc;           ///< Heading accuracy estimate [1e-5 deg]
    uint16_t pDOP;              ///< Position DOP [0.01]
    uint8_t flags3;
    uint8_t reserved1[5];
    int32_t headVeh;            ///< Heading of vehicle (2-D) [1e-5 deg]
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
static_assert(sizeof(UbxNavPvtRaw_t) == 100, "Wrong size");
