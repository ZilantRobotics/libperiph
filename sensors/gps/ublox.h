/*
 * Copyright (C) 2019-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_GPS_UBLOX_H_
#define SENSORS_GPS_UBLOX_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "ubx/nav_pvt.h"
#include "ubx/nav_status.h"
#include "ubx/nav_cov.h"

typedef enum {
    UBX_UNKNOWN_PKG = 0,
    UBX_NAV_PVT_PKG,
    UBX_NAV_STATUS_PKG,
    UBX_NAV_COV_PKG,
    // UBX_NAV_RELPOSNED_PKG,

    UBX_PACKAGE_TYPES_AMOUNT,
} UbloxPackageType_t;

/**
 * @brief uavcan.equipment.gnss.Fix2
 * @note GNSS ECEF and LLA navigation solution with uncertainty.
 */
typedef enum {
    GPS_UBLOX_STATUS_NO_FIX = 0,
    GPS_UBLOX_STATUS_TIME_ONLY = 1,
    GPS_UBLOX_STATUS_2D_FIX = 2,
    GPS_UBLOX_STATUS_3D_FIX = 3,
} GnssUbloxStatus_t;

typedef struct {
    uint64_t timestamp;
    uint64_t gnss_timestamp;
    uint8_t gnss_time_standard;
    uint8_t num_leap_seconds;
    int64_t longitude_deg_1e8;
    int64_t latitude_deg_1e8;
    int32_t height_ellipsoid_mm;
    int32_t height_msl_mm;
    float ned_velocity[3];
    uint8_t sats_used;
    GnssUbloxStatus_t status;

    uint8_t mode;
    uint8_t sub_mode;
    uint8_t covariance_len;
    float covariance[36];

    float pdop;
} GnssUblox_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse raw ublox buffer and save data to uavcan fix2 buffer in success.
 * @param gns_buffer is an input raw buffer (or his part)
 * @param gns_buffer_size is a size of this buffer
 * @param uavcan_fix2 is an output buffer
 * @return package type, if package successfully has been parsed
 * @note parser is stateful
 */
UbloxPackageType_t ubloxParse(const uint8_t buffer[], size_t size, size_t* num_of_parsed_bytes);


void ubloxGetUbxNavPvt(UbxNavPvt_t* ubx_nav_pvt);
void ubloxGetUbxNavStatus(UbxNavStatus_t* ubx_nav_status);
void ubloxGetUbxNavCov(UbxNavCov_t* ubloxGetUbxNavCov);

void ubloxGetDroneCanFix2(GnssUblox_t* uavcan_fix2);

uint16_t ubloxGetPackageStats(UbloxPackageType_t package_type);

double ubloxRawToRad(int32_t ublox_raw_deg_1e_7);
uint64_t dayToUnixTimestamp(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi, uint8_t s);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_GPS_UBLOX_H_
