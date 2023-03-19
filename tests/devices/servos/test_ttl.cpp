/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file test_ttl.cpp
 * @author d.ponomarev
 * @date Aug 10, 2022
 */

#include <gtest/gtest.h>
#include <iostream>
#include "ttl.h"

TEST(TTL, test_set_get_timeout) {
    ttlSetTimeout(1000);
    ASSERT_EQ(ttlGetTimeout(), 1000);

    ttlSetTimeout(2000);
    ASSERT_EQ(ttlGetTimeout(), 2000);
}

TEST(TTL, test_set_get_setpoint_timestamp) {
    ttlSetSetpointTimestamp(0, 1000);
    ttlSetSetpointTimestamp(10, 2000);
    ttlSetSetpointTimestamp(19, 3000);

    ASSERT_EQ(1000, ttlGetSetpointTimestamp(0));
    ASSERT_EQ(2000, ttlGetSetpointTimestamp(10));
    ASSERT_EQ(3000, ttlGetSetpointTimestamp(19));

    // bad argument should not affect on the application, getter should always return 0
    ttlSetSetpointTimestamp(20, 4000);
    ASSERT_EQ(0, ttlGetSetpointTimestamp(20));
}

TEST(TTL, test_is_alive) {
    ttlSetTimeout(1000);
    ttlSetSetpointTimestamp(0, 1000);
    ttlSetSetpointTimestamp(10, 2000);
    ttlSetSetpointTimestamp(19, 3000);

    ASSERT_EQ(true, ttlIsSetpointAlive(0, 1999));
    ASSERT_EQ(false, ttlIsSetpointAlive(0, 2001));

    ASSERT_EQ(true, ttlIsSetpointAlive(10, 2999));
    ASSERT_EQ(false, ttlIsSetpointAlive(10, 3001));

    ASSERT_EQ(true, ttlIsSetpointAlive(19, 3999));
    ASSERT_EQ(false, ttlIsSetpointAlive(19, 4001));
}

TEST(TTL, test_best_timestamp_and_alive) {
    ttlSetTimeout(1000);
    for (uint8_t sp_idx = 0; sp_idx < 20; sp_idx++) {
        ttlSetSetpointTimestamp(sp_idx, 0);
    }

    ttlSetSetpointTimestamp(0, 1000);
    ttlSetSetpointTimestamp(10, 2000);
    ttlSetSetpointTimestamp(19, 1000);

    ASSERT_EQ(2000, ttlGetBestTimestamp());

    ASSERT_EQ(true, ttlIsBestSetpointAlive(2999));
    ASSERT_EQ(false, ttlIsBestSetpointAlive(3001));
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
