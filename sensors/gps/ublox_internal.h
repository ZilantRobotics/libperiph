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
#include "ubx/nav_pvt.h"
#include "ubx/nav_status.h"

#define GPS_UBLOX_SYNC_CHAR_1_CODE          0xB5    // 181
#define GPS_UBLOX_SYNC_CHAR_2_CODE          0x62    // 98

#define GPS_UBLOX_MAX_PACKAGE_SIZE          200

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
///< it should be packed to correctly perform crc calculation
// *INDENT-OFF*
typedef enum __attribute__((__packed__)) {
    CLASS_NAV = 0x01
} UbloxClass_t;
// *INDENT-ON*

///< use compile attribute otherwise a value of this type will be 4 bytes in size
///< it should be packed to correctly perform crc calculation
// *INDENT-OFF*
typedef enum __attribute__((__packed__)) {
    ID_NAV_STATUS = 0x03,
    ID_NAV_PVT = 0x07,
    ID_NAV_COV = 0x36,
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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Private functions. For tests only
 */
uint16_t ubloxCrc16(const uint8_t* buf, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_GPS_UBLOX_INTERNAL_H_
