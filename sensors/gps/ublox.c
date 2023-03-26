/*
 * Copyright (C) 2019-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ublox.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "ublox_internal.h"


static UbloxPackage_t package = {};

static void ubloxCrcAddByte(uint8_t byte);
static bool ubloxCheckCrc();
static void ubloxClearCrc();
static bool ubloxNextByte(uint8_t byte);
static void ubloxDeserializeFix2(GnssUblox_t* uavcan_fix2);

uint64_t dayToUnixTimestamp(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi, uint8_t s) {
    struct tm time;
    time.tm_sec = s;
    time.tm_min = mi;
    time.tm_hour = h;
    time.tm_mday = d;
    time.tm_mon = mo - 1;
    time.tm_year = y - 1900;
    time_t utc_time = mktime(&time);
    return (uint64_t)utc_time;
}

UbloxPackageType_t ubloxParse(const uint8_t buffer[], size_t size) {
    if (buffer == NULL || size > 256) {
        return UBX_UNKNOWN_PKG;
    }

    UbloxPackageType_t received_package = UBX_UNKNOWN_PKG;
    for (uint32_t idx = 0; idx < size; idx++) {
        if (ubloxNextByte(buffer[idx]) && ubloxCheckCrc()) {
            if (package.id == ID_NAV_PVT) {
                received_package = UBX_NAV_PVT_PKG;
            } else if (package.id == ID_NAV_STATUS) {
                received_package = UBX_NAV_STATUS_PKG;
            }
        }
    }

    return received_package;
}

void ubloxGetDroneCanFix2(GnssUblox_t* uavcan_fix2) {
    if (uavcan_fix2 == NULL) {
        return;
    }

    ubloxDeserializeFix2(uavcan_fix2);
}

void ubloxGetUbxNavStatus(UbxNavStatus_t* ubx_nav_status) {
    if (ubx_nav_status == NULL || sizeof(UbxNavStatus_t) != package.length) {
        return;
    }

    memcpy(ubx_nav_status, (const void*)package.payload, sizeof(UbxNavStatus_t));
}

/**
 * @return true when package is finished, otherwise false
 */
bool ubloxNextByte(uint8_t byte) {
    switch (package.state) {
        case STATE_SYNC_CHAR_1:
            if (byte == GPS_UBLOX_SYNC_CHAR_1_CODE) {
                package.state = STATE_SYNC_CHAR_2;
            }
            break;
        case STATE_SYNC_CHAR_2:
            package.state = (byte == GPS_UBLOX_SYNC_CHAR_2_CODE) ? STATE_CLASS : STATE_SYNC_CHAR_1;
            break;
        case STATE_CLASS:
            package.ubx_class = (UbloxClass_t)byte;
            package.state = (byte == CLASS_NAV) ? STATE_ID : STATE_SYNC_CHAR_1;
            break;
        case STATE_ID:
            package.id = (UbloxId_t)byte;
            if (package.id == ID_NAV_PVT || package.id == ID_NAV_STATUS) {
                package.state = STATE_LENGTH_1;
            } else {
                package.state = STATE_SYNC_CHAR_1;
            }
            break;
        case STATE_LENGTH_1:
            package.length = byte;
            if ((package.id == ID_NAV_PVT && package.length == sizeof(UbxNavPvt_t)) ||
                (package.id == ID_NAV_STATUS && package.length == sizeof(UbxNavStatus_t))) {
                package.state = STATE_LENGTH_2;
            } else {
                package.state = STATE_SYNC_CHAR_1;
            }
            break;
        case STATE_LENGTH_2:
            package.length += (byte << 8);
            if (byte == 0) {
                package.state = STATE_PAYLOAD;
                package.payload_counter = 0;
            } else {
                package.state = STATE_SYNC_CHAR_1;
            }
            break;
        case STATE_PAYLOAD:
            if (package.payload_counter + 1 >= package.length) {
                package.state = STATE_CHECKSUM_A;
            }
            package.payload[package.payload_counter++] = byte;
            break;
        case STATE_CHECKSUM_A:
            package.crc = byte;
            package.state = STATE_CHECKSUM_B;
            break;
        case STATE_CHECKSUM_B:
            package.crc += (byte << 8);
            package.state = STATE_SYNC_CHAR_1;
            return true;
        default:
            break;
    }
    return false;
}


void ubloxDeserializeFix2(GnssUblox_t* uavcan_fix2) {
    if (sizeof(UbxNavPvt_t) != package.length) {
        return;
    }
    UbxNavPvt_t* ubx_nav_pvt = (UbxNavPvt_t*)(void*)package.payload;
    uavcan_fix2->timestamp = (uint64_t)(ubx_nav_pvt->time_of_week_ms) * 1000;
    uavcan_fix2->gnss_timestamp = dayToUnixTimestamp(ubx_nav_pvt->year_utc,
                                                     ubx_nav_pvt->month_utc,
                                                     ubx_nav_pvt->day_utc,
                                                     ubx_nav_pvt->hour_utc,
                                                     ubx_nav_pvt->min_utc,
                                                     ubx_nav_pvt->sec_utc) * 1000000;
    uavcan_fix2->gnss_time_standard = 0;
    uavcan_fix2->num_leap_seconds = 0;
    uavcan_fix2->longitude_deg_1e8 = ((int64_t)ubx_nav_pvt->lon) * 10;
    uavcan_fix2->latitude_deg_1e8 = ((int64_t)ubx_nav_pvt->lat) * 10;
    uavcan_fix2->height_ellipsoid_mm = ubx_nav_pvt->height;
    uavcan_fix2->height_msl_mm = ubx_nav_pvt->hMSL;
    uavcan_fix2->ned_velocity[0] = ubx_nav_pvt->velN / 1e3F;
    uavcan_fix2->ned_velocity[1] = ubx_nav_pvt->velE / 1e3F;
    uavcan_fix2->ned_velocity[2] = ubx_nav_pvt->velD / 1e3F;
    uavcan_fix2->sats_used = ubx_nav_pvt->numSV;
    uavcan_fix2->status = (GnssUbloxStatus_t)ubx_nav_pvt->fixType;
    uavcan_fix2->pdop = ubx_nav_pvt->pDOP / 100;
}

bool ubloxCheckCrc() {
    const uint8_t* buf = (uint8_t*)(&package.ubx_class);
    size_t size = package.length + 4;
    return ubloxCrc16(buf, size) == package.crc;
}

uint16_t ubloxCrc16(const uint8_t* buf, size_t size) {
    ubloxClearCrc();

    while (size-- != 0) {
        ubloxCrcAddByte(*buf++);
    }
    return package.crc_checker.u16;
}

void ubloxClearCrc() {
    package.crc_checker.u16 = 0;
}

void ubloxCrcAddByte(uint8_t byte) {
    package.crc_checker.bytes.crc_a += byte;
    package.crc_checker.bytes.crc_b += package.crc_checker.bytes.crc_a;
}
