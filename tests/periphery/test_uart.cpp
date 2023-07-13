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


TEST(hal_uart, test) {
    ASSERT_EQ(LIBPERIPH_OK, uartInitRxDma(UART_FIRST, NULL, 0));
    ASSERT_EQ(0, uartGetLastReceivedIndex(UART_FIRST));
    ASSERT_EQ(NULL, uartRxDmaPop());
    ASSERT_EQ(LIBPERIPH_OK, uartTransmit(NULL, 0));
    ASSERT_EQ(LIBPERIPH_OK, uartTransmitDma(NULL, 0));
    ASSERT_EQ(false, uartIsReady());
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
