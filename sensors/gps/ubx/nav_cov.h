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
    uint32_t iTow;                  ///< GPS time of week of the navigation epoch
    uint8_t version;
    uint8_t posCovValid;
    uint8_t velCovValid;
    uint8_t reserved1[9];

    float posCovNN;
    float posCovNE;
    float posCovND;
    float posCovEE;
    float posCovED;
    float posCovDD;

    float velCovNN;
    float velCovNE;
    float velCovND;
    float velCovEE;
    float velCovED;
    float velCovDD;
} UbxNavCov_t;
static_assert(sizeof(UbxNavCov_t) == 64, "Wrong size");

#pragma pack(push, 1)
typedef struct {
    uint8_t sync_char_1;
    uint8_t sync_char_2;
    uint8_t class_nav;
    uint8_t id_nav_pvt;
    uint16_t payload_length;
    UbxNavCov_t payload;
    uint16_t crc;
} UbxNavCovRaw_t;
#pragma pack(pop)
static_assert(sizeof(UbxNavCovRaw_t) == 72, "Wrong size");
