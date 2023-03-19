/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ttl.c
 * @author d.ponomarev
 * @date Aug 10, 2022
 */

#include "ttl.h"

#define SETPOINTS_AMOUNT 20

static uint32_t setpoints_ts_ms[SETPOINTS_AMOUNT] = {};
static uint32_t ttl_ms = 500;


/// TTL timeout
void ttlSetTimeout(uint32_t _ttl_ms) {
    ttl_ms = _ttl_ms;
}
uint32_t ttlGetTimeout() {
    return ttl_ms;
}

/// Setpoints
void ttlSetSetpointTimestamp(uint8_t sp_idx, uint32_t crnt_time_ms) {
    if (sp_idx >= SETPOINTS_AMOUNT) {
        return;
    }
    setpoints_ts_ms[sp_idx] = crnt_time_ms;
}

uint32_t ttlGetSetpointTimestamp(uint8_t sp_idx) {
    return (sp_idx < SETPOINTS_AMOUNT) ? setpoints_ts_ms[sp_idx] : 0;
}

uint32_t ttlGetBestTimestamp() {
    uint32_t best_ts_ms = 0;
    for (uint8_t sp_idx = 0; sp_idx < SETPOINTS_AMOUNT; sp_idx++) {
        if (best_ts_ms < setpoints_ts_ms[sp_idx]) {
            best_ts_ms = setpoints_ts_ms[sp_idx];
        }
    }
    return best_ts_ms;
}

bool ttlIsSetpointAlive(uint8_t sp_idx, uint32_t crnt_time_ms) {
    if (sp_idx >= SETPOINTS_AMOUNT) {
        return false;
    }
    return setpoints_ts_ms[sp_idx] + ttl_ms > crnt_time_ms;
}

bool ttlIsBestSetpointAlive(uint32_t crnt_time_ms) {
    uint32_t best_ts_ms = ttlGetBestTimestamp();
    return best_ts_ms + ttl_ms > crnt_time_ms;
}
