/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file gps_ublox.c
 * @author d.ponomarev
 */


#include "ublox.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


static UbloxPackage_t package = {};

static void crc_add_byte(uint8_t byte);
static bool ubloxCheckCrc();
static void ubloxClearCrc();
static bool ubloxNextByte(uint8_t byte);
static void ubloxDeserializeFix2(GnssFix2_t* uavcan_fix2);

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

bool ubloxParseFix2(const uint8_t gns_buffer[], size_t gns_buffer_size, GnssFix2_t* uavcan_fix2) {
    if (gns_buffer == NULL || gns_buffer_size > 256 || uavcan_fix2 == NULL) {
        return false;
    }

    bool is_package_parsed = false;
    for (uint32_t idx = 0; idx < gns_buffer_size; idx++) {
        if (ubloxNextByte(gns_buffer[idx])) {
            if (ubloxCheckCrc()) {
                if (package.id == ID_NAV_PVT) {
                    ubloxDeserializeFix2(uavcan_fix2);
                    is_package_parsed = true;
                }
            }
        }
    }
    return is_package_parsed;
}

bool ubloxNextByte(uint8_t byte) {
    switch (package.state) {
        case STATE_SYNC_CHAR_1:
            if (byte == SYNC_CHAR_1_CODE) {
                package.state = STATE_SYNC_CHAR_2;
            }
            break;
        case STATE_SYNC_CHAR_2:
            package.state = (byte == SYNC_CHAR_2_CODE) ? STATE_CLASS : STATE_SYNC_CHAR_1;
            break;
        case STATE_CLASS:
            package.ubx_class = (UbloxClass_t)byte;
            package.state = (byte == CLASS_NAV) ? STATE_ID : STATE_SYNC_CHAR_1;
            break;
        case STATE_ID:
            package.id = (UbloxId_t)byte;
            package.state = (package.id == ID_NAV_PVT) ? STATE_LENGTH_1 : STATE_SYNC_CHAR_1;
            break;
        case STATE_LENGTH_1:
            package.length = byte;
            if (package.length == sizeof(UbxNavPvt_t)) {
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


void ubloxDeserializeFix2(GnssFix2_t* uavcan_fix2) {
    if (sizeof(UbxNavPvt_t) != package.length) {
        return;
    }
    UbxNavPvt_t* ubx_nav_pvt = (UbxNavPvt_t*)package.payload;
    uavcan_fix2->timestamp = (uint64_t)(ubx_nav_pvt->time_ms) * 1000;
    uavcan_fix2->gnss_timestamp = dayToUnixTimestamp(ubx_nav_pvt->year,
                                                     ubx_nav_pvt->month,
                                                     ubx_nav_pvt->day,
                                                     ubx_nav_pvt->hour,
                                                     ubx_nav_pvt->min,
                                                     ubx_nav_pvt->sec) * 1000000;
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
    uavcan_fix2->status = (Fix2_status_t)ubx_nav_pvt->fixType;
    uavcan_fix2->pdop = ubx_nav_pvt->pDOP / 100;
}

void ubloxConvertFix2ToNavPvt(UbxNavPvtRaw_t* buffer, const GnssFix2_t* uavcan_fix2) {
    buffer->sync_char_1 = SYNC_CHAR_1_CODE;
    buffer->sync_char_2 = SYNC_CHAR_2_CODE;
    buffer->class_nav = CLASS_NAV;
    buffer->id_nav_pvt = ID_NAV_PVT;
    buffer->payload_length = sizeof(UbxNavPvt_t);

    buffer->payload.time_ms = uavcan_fix2->timestamp / 1000;

    ///< @todo reverse dayToUnixTimestamp() should be here
    buffer->payload.year = 0;
    buffer->payload.month = 0;
    buffer->payload.day = 0;
    buffer->payload.hour = 0;
    buffer->payload.min = 0;
    buffer->payload.sec = 0;

    buffer->payload.lon = uavcan_fix2->longitude_deg_1e8 / 10;
    buffer->payload.lat = uavcan_fix2->latitude_deg_1e8 / 10;
    buffer->payload.height = uavcan_fix2->height_ellipsoid_mm;
    buffer->payload.hMSL = uavcan_fix2->height_msl_mm;
    buffer->payload.velN = uavcan_fix2->ned_velocity[0] * 1e3F;
    buffer->payload.velE = uavcan_fix2->ned_velocity[1] * 1e3F;
    buffer->payload.velD = uavcan_fix2->ned_velocity[2] * 1e3F;
    buffer->payload.numSV = uavcan_fix2->sats_used;
    buffer->payload.fixType = uavcan_fix2->status;
    buffer->payload.pDOP = uavcan_fix2->pdop * 100;

    buffer->crc = ubloxCrc16((uint8_t*)(&buffer->class_nav), buffer->payload_length + 4);
}

bool ubloxCheckCrc() {
    const uint8_t* buf = (uint8_t*)(&package.ubx_class);
    size_t size = package.length + 4;
    return ubloxCrc16(buf, size) == package.crc;
}

uint16_t ubloxCrc16(const uint8_t* buf, size_t size) {
    ubloxClearCrc();

    while (size-- != 0) {
        crc_add_byte(*buf++);
    }
    uint16_t calculated_crc = package.crc_checker.crc_a + (package.crc_checker.crc_b << 8);
    return calculated_crc;
}

void ubloxClearCrc() {
    package.crc_checker.crc_a = 0;
    package.crc_checker.crc_b = 0;
}

void crc_add_byte(uint8_t byte) {
    package.crc_checker.crc_a += byte;
    package.crc_checker.crc_b += package.crc_checker.crc_a;
}