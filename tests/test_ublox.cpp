/*
 * Copyright (C) 2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file test_ublox.cpp
 * @author d.ponomarev
 */

#include <gtest/gtest.h>
#include <iostream>
#include "gps/ublox.h"

void create_correct_ubx_nav_pvt_package(UbxNavPvtRaw_t& out_pkg) {
    UbxNavPvtRaw_t ubx_nav_pvt = {
        .sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE,
        .sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE,
        .class_nav = CLASS_NAV,
        .id_nav_pvt = ID_NAV_PVT,
        .payload_length = sizeof(UbxNavPvt_t),
    };
    ubx_nav_pvt.crc = ubloxCrc16(&ubx_nav_pvt.class_nav, sizeof(ubx_nav_pvt) - 4);
    out_pkg = ubx_nav_pvt;
}

void create_ubx_nav_pvt_package_with_wrong_crc(UbxNavPvtRaw_t& out_pkg) {
    UbxNavPvtRaw_t ubx_nav_pvt = {
        .sync_char_1 = GPS_UBLOX_SYNC_CHAR_1_CODE,
        .sync_char_2 = GPS_UBLOX_SYNC_CHAR_2_CODE,
        .class_nav = CLASS_NAV,
        .id_nav_pvt = ID_NAV_PVT,
        .payload_length = sizeof(UbxNavPvt_t),
        .payload = {},
        .crc = 0x0000,  ///< wrong crc
    };
    out_pkg = ubx_nav_pvt;
}


TEST(Ublox, test_serialization_01_correct_package) {
    GnssUblox_t uavcan_fix2;
    UbxNavPvtRaw_t ubx_nav_pvt_raw;
    create_correct_ubx_nav_pvt_package(ubx_nav_pvt_raw);

    auto is_parsed = ubloxParseFix2((uint8_t*)&ubx_nav_pvt_raw, sizeof(ubx_nav_pvt_raw), &uavcan_fix2);

    ASSERT_EQ(is_parsed, true);
}

TEST(Ublox, test_serialization_02_correct_package) {
    GnssUblox_t uavcan_fix2;
    UbxNavPvtRaw_t ubx_nav_pvt_raw;
    create_ubx_nav_pvt_package_with_wrong_crc(ubx_nav_pvt_raw);

    auto is_parsed = ubloxParseFix2((uint8_t*)&ubx_nav_pvt_raw, sizeof(ubx_nav_pvt_raw), &uavcan_fix2);

    ASSERT_EQ(is_parsed, false);
}

TEST(Ublox, test_serialization_03_two_packages) {
    GnssUblox_t uavcan_fix2;
    UbxNavPvtRaw_t ubx_nav_pvt_raw;
    create_correct_ubx_nav_pvt_package(ubx_nav_pvt_raw);

    uint8_t buffer[200] = {};
    memcpy(buffer, &ubx_nav_pvt_raw, 100);
    memcpy(buffer + 100, &ubx_nav_pvt_raw, 100);

    ASSERT_EQ(ubloxParseFix2(buffer, 100, &uavcan_fix2), true);
    ASSERT_EQ(ubloxParseFix2(buffer + 100, 100, &uavcan_fix2), true);
}

TEST(Ublox, test_serialization_04_two_packages) {
    GnssUblox_t uavcan_fix2;
    UbxNavPvtRaw_t ubx_nav_pvt_raw;
    create_correct_ubx_nav_pvt_package(ubx_nav_pvt_raw);

    uint8_t buffer[200] = {};
    memcpy(buffer, &ubx_nav_pvt_raw, 100);
    memcpy(buffer + 100, &ubx_nav_pvt_raw, 100);

    ASSERT_EQ(ubloxParseFix2(buffer + 0,    50, &uavcan_fix2), false);
    ASSERT_EQ(ubloxParseFix2(buffer + 50,   50, &uavcan_fix2), true);
    ASSERT_EQ(ubloxParseFix2(buffer + 100,  50, &uavcan_fix2), false);
    ASSERT_EQ(ubloxParseFix2(buffer + 150,  50, &uavcan_fix2), true);
}

TEST(Ublox, test_serialization_05_two_packages) {
    GnssUblox_t uavcan_fix2;
    UbxNavPvtRaw_t ubx_nav_pvt_raw;
    create_correct_ubx_nav_pvt_package(ubx_nav_pvt_raw);

    uint8_t buffer[200] = {};
    memcpy(buffer, &ubx_nav_pvt_raw, 100);
    memcpy(buffer + 100, &ubx_nav_pvt_raw, 100);

    ASSERT_EQ(ubloxParseFix2(buffer + 0,    50, &uavcan_fix2), false);
    ASSERT_EQ(ubloxParseFix2(buffer + 50,   100, &uavcan_fix2), true);
    ASSERT_EQ(ubloxParseFix2(buffer + 150,  50, &uavcan_fix2), true);
}

TEST(Ublox, test_deserialization_01_correct) {
    UbxNavPvtRaw_t ubx_nav_pvt_raw_expected;
    create_correct_ubx_nav_pvt_package(ubx_nav_pvt_raw_expected);

    GnssUblox_t uavcan_fix2 = {};
    UbxNavPvtRaw_t ubx_nav_pvt_raw_actual = {};
    ubloxConvertFix2ToNavPvt(&ubx_nav_pvt_raw_actual, &uavcan_fix2);

    ASSERT_EQ(ubx_nav_pvt_raw_expected.sync_char_1, ubx_nav_pvt_raw_actual.sync_char_1);
    ASSERT_EQ(ubx_nav_pvt_raw_expected.sync_char_2, ubx_nav_pvt_raw_actual.sync_char_2);
    ASSERT_EQ(ubx_nav_pvt_raw_expected.class_nav, ubx_nav_pvt_raw_actual.class_nav);
    ASSERT_EQ(ubx_nav_pvt_raw_expected.id_nav_pvt, ubx_nav_pvt_raw_actual.id_nav_pvt);

    auto memcmp_result = memcmp(&ubx_nav_pvt_raw_expected.payload,
                                &ubx_nav_pvt_raw_actual.payload,
                                sizeof(UbxNavPvt_t));
    ASSERT_EQ(memcmp_result, 0);

    ASSERT_EQ(ubx_nav_pvt_raw_expected.crc, ubx_nav_pvt_raw_actual.crc);

    auto is_parsed = ubloxParseFix2((uint8_t*)&ubx_nav_pvt_raw_actual,
                                    sizeof(ubx_nav_pvt_raw_actual),
                                    &uavcan_fix2);
    ASSERT_EQ(is_parsed, true);
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
