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
