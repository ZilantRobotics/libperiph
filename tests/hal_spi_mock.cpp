/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file hal_spi_mock.cpp
 * @author d.ponomarev
 */

#include <stdint.h>

int8_t spiPerformTransaction(uint8_t* tx, uint8_t* rx, uint8_t size) {
    (void)tx;
    (void)rx;
    (void)size;
    return 0;
}
