/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "hal_uart.h"
#include "libperiph_common.h"

TEST(hal_uart, test_uartInitRxDma) {
    uint8_t uart_rx_buffer[8];
    ASSERT_EQ(LIBPERIPH_OK, uartInitRxDma(UART_FIRST, uart_rx_buffer, 8));

    ASSERT_EQ(LIBPERIPH_ERROR, uartInitRxDma(UART_AMOUNT, uart_rx_buffer, 8));
    ASSERT_EQ(LIBPERIPH_ERROR, uartInitRxDma(UART_FIRST, NULL, 8));
    ASSERT_EQ(LIBPERIPH_ERROR, uartInitRxDma(UART_FIRST, uart_rx_buffer, 0));
}

TEST(hal_uart, test_uartGetLastReceivedIndex) {
    ASSERT_EQ(0, uartGetLastReceivedIndex(UART_FIRST));
    ASSERT_EQ(0, uartGetLastReceivedIndex(UART_SECOND));

    ASSERT_EQ(0, uartGetLastReceivedIndex(UART_AMOUNT));
}

TEST(hal_uart, test_uartTransmit) {
    uint8_t uart_tx_buffer[8];
    ASSERT_EQ(LIBPERIPH_OK, uartTransmit(uart_tx_buffer, 8));

    ASSERT_EQ(LIBPERIPH_ERROR, uartTransmit(NULL, 8));
    ASSERT_EQ(LIBPERIPH_ERROR, uartTransmit(uart_tx_buffer, 0));
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
