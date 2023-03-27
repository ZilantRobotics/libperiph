/*
 * Copyright (C) 2019-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdint.h>
#include <assert.h>

///< use compile attribute otherwise a value of this type will be 4 bytes in size
///< it should be packed to correctly perform crc calculation
// *INDENT-OFF*
typedef enum __attribute__((__packed__)) {
    gpsFixOk = (1 << 0),        ///< position and velocity valid and within DOP and ACC Masks
    diffSoln = (1 << 1),        ///< differential corrections were applied
    wknSet = (1 << 2),          ///< Week Number valid (see Time Validity section for details)
    towSet = (1 << 3),          ///< Time of Week valid (see Time Validity section for details)
} UbxNavStatusFlags_t;
// *INDENT-ON*

///< use compile attribute otherwise a value of this type will be 4 bytes in size
///< it should be packed to correctly perform crc calculation
// *INDENT-OFF*
typedef enum __attribute__((__packed__)) {
    UNKNOWN = 0,
    NO_SPOOFING_INDICATED = 1,
    SPOOFING_INDICATED = 2,
    MULTIPLE_SPOOFING_INDICATIONS = 3,
} SpoofingDetectionState_t;
// *INDENT-ON*

typedef union {
    uint8_t psmState : 2;
    uint8_t reserver1 : 1;
    SpoofingDetectionState_t spoofDetState : 2;
    uint8_t reserver2 : 1;
    uint8_t carrSoln : 2;
} UbxNavStatusFlags2_t;

typedef struct {
    uint32_t iTow;                  ///< GPS time of week of the navigation epoch
    uint8_t gpsFix;
    UbxNavStatusFlags_t flags;      ///< Navigation Status Flags
    uint8_t fixStat;                ///< Fix Status Information
    UbxNavStatusFlags2_t flags2;    ///< further information about navigation output
    uint32_t ttff;                  ///< Time to first fix
    uint32_t msss;                  ///< Milliseconds since Startup / Reset
} UbxNavStatus_t;
static_assert(sizeof(UbxNavStatus_t) == 16, "Wrong size");
