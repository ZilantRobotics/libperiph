/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "hal_spi.h"
#include "libperiph_common.h"

#define RM3100_CHECK_REG_CCX (0x80 | 0x04)

const uint8_t RM3100_CCX_CONTENT[6] = {0, 200, 0, 200, 0, 200};

int8_t ubuntuSpiCallback(const uint8_t* tx, uint8_t* rx, uint8_t size) {
    if (tx[0] == RM3100_CHECK_REG_CCX) {
        memcpy(rx + 1, RM3100_CCX_CONTENT, std::min((uint8_t)6, (uint8_t)size));
    }

    return LIBPERIPH_OK;
}

TEST(hal_spi, test_spiPerformTransaction) {
    uint8_t tx_buf[8];
    uint8_t rx_buf[8];
    ASSERT_EQ(LIBPERIPH_OK, spiPerformTransaction(tx_buf, rx_buf, 8));

    ASSERT_EQ(LIBPERIPH_ERROR, spiPerformTransaction(NULL, rx_buf, 8));
    ASSERT_EQ(LIBPERIPH_ERROR, spiPerformTransaction(tx_buf, NULL, 8));
    ASSERT_EQ(LIBPERIPH_ERROR, spiPerformTransaction(tx_buf, rx_buf, 0));
}

TEST(hal_spi, test_rm3100_example) {
    std::array<uint8_t, 8> tx_buf = {RM3100_CHECK_REG_CCX};
    std::array<uint8_t, 8> rx_buf;
    ASSERT_EQ(LIBPERIPH_OK, spiPerformTransaction((uint8_t*)&tx_buf, (uint8_t*)&rx_buf, 8));

    ASSERT_TRUE(std::equal(std::begin(rx_buf) + 1,
                std::end(rx_buf) - 1,
                std::begin(RM3100_CCX_CONTENT)));
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
