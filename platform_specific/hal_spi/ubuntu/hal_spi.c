/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <hal_spi.h>
#include <stddef.h>
#include "libperiph_common.h"

// NOTE: This function Should not be modified, when the callback is neede
__attribute__((weak)) int8_t ubuntuSpiCallback(const uint8_t* tx, uint8_t* rx, uint8_t size) {
    (void)(tx);
    (void)(rx);
    (void)(size);
    return LIBPERIPH_OK;
}

int8_t spiPerformTransaction(uint8_t* tx, uint8_t* rx, uint8_t size) {
    if (tx == NULL || rx == NULL || size == 0) {
        return LIBPERIPH_ERROR;
    }

    return ubuntuSpiCallback(tx, rx, size);
}
