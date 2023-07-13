/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "hal_i2c.h"
#include "libperiph_common.h"


TEST(hal_i2c, test) {
    ASSERT_EQ(LIBPERIPH_OK, i2cTransmit(0, NULL, 0));
    ASSERT_EQ(LIBPERIPH_ERROR, i2cReceive(0, NULL, 0));
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
