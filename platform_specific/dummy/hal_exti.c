/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_exti.h"

int8_t extiInit() {
    return 0;
}

bool extiIsInited() {
    return true;
}

bool extiWaitForInterrupt_12_15(uint16_t block_time_ms) {
    return false;
}

bool extiWaitForInterrupt_0_11(uint16_t block_time_ms) {
    return false;
}
