/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ublox_emulation.h"

void ubloxCreateRawDataFromPackage(UbxNavPvtRaw_t* buffer, const GnssUblox_t* uavcan_fix2) {
    buffer->sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE;
    buffer->sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE;
    buffer->class_nav = CLASS_NAV;
    buffer->id_nav_pvt = ID_NAV_PVT;
    buffer->payload_length = sizeof(UbxNavPvt_t);

    buffer->payload.time_of_week_ms = uavcan_fix2->timestamp / 1000;

    ///< @todo reverse dayToUnixTimestamp() should be here
    buffer->payload.year_utc = 0;
    buffer->payload.month_utc = 0;
    buffer->payload.day_utc = 0;
    buffer->payload.hour_utc = 0;
    buffer->payload.min_utc = 0;
    buffer->payload.sec_utc = 0;

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
