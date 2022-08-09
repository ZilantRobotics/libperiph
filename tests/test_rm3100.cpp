/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file test_rm3100.cpp
 * @author d.ponomarev
 */

#include <iostream>
#include <gtest/gtest.h>
#include "magnetometer/rm3100.h"


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
