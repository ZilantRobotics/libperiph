/*
 * Copyright (C) 2019-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ublox_internal.h
 * @author d.ponomarev
 */

#ifndef SENSORS_GPS_UBLOX_INTERNAL_H_
#define SENSORS_GPS_UBLOX_INTERNAL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#define GPS_UBLOX_SYNC_CHAR_1_CODE        0xB5    // 181
#define GPS_UBLOX_SYNC_CHAR_2_CODE        0x62    // 98

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
static_assert(sizeof(UbxNavPvt_t) == 92, "Wrong UbxNavPvt_t size");

typedef enum {
    STATE_SYNC_CHAR_1 = 0,
    STATE_SYNC_CHAR_2,
    STATE_CLASS,
    STATE_ID,
    STATE_LENGTH_1,
    STATE_LENGTH_2,
    STATE_PAYLOAD,
    STATE_CHECKSUM_A,
    STATE_CHECKSUM_B,
} UbloxState_t;

///< use compile attribute otherwise a value of this type will be 4 bytes in size
///< it should be packed to correctly perform crc calculatation
// *INDENT-OFF*
typedef enum __attribute__((__packed__)) {
    CLASS_NAV = 0x01
} UbloxClass_t;
// *INDENT-ON*

///< use compile attribute otherwise a value of this type will be 4 bytes in size
///< it should be packed to correctly perform crc calculatation
// *INDENT-OFF*
typedef enum __attribute__((__packed__)) {
    ID_NAV_PVT = 0x07,
} UbloxId_t;
// *INDENT-ON*

typedef struct {
    uint8_t crc_a;
    uint8_t crc_b;
} UbloxCrcChecker_t;

typedef union {
    UbloxCrcChecker_t bytes;
    uint16_t u16;
} UbloxCrcCheckerUnion_t;

#pragma pack(push, 1)
typedef struct {
    UbloxState_t state;
    UbloxClass_t ubx_class;
    UbloxId_t id;
    uint16_t length;
    uint8_t payload[128];
    uint16_t crc;
    uint8_t payload_counter;
    UbloxCrcCheckerUnion_t crc_checker;
} UbloxPackage_t;
#pragma pack(pop)

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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Private functions. For tests only
 */
uint16_t ubloxCrc16(const uint8_t* buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_GPS_UBLOX_INTERNAL_H_
