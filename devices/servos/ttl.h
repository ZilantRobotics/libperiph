/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file ttl.h
 * @author d.ponomarev
 * @date Aug 10, 2022
 */

#ifndef DEVICES_SERVOS_TTL_H_
#define DEVICES_SERVOS_TTL_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// TTL timeout
void ttlSetTimeout(uint32_t ttl_ms);
uint32_t ttlGetTimeout();

/// Setpoints
void ttlSetSetpointTimestamp(uint8_t sp_idx, uint32_t crnt_time_ms);
uint32_t ttlGetSetpointTimestamp(uint8_t sp_idx);
uint32_t ttlGetBestTimestamp();

bool ttlIsSetpointAlive(uint8_t sp_idx, uint32_t crnt_time_ms);
bool ttlIsBestSetpointAlive(uint32_t crnt_time_ms);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_SERVOS_TTL_H_
