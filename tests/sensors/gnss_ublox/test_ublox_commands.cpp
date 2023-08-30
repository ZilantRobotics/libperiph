/*
 * Copyright (C) 2021-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>
#include <iostream>
#include "gps/ublox.h"
#include "gps/ublox_commands.h"
#include "gps/ublox_emulation.h"

typedef int8_t (*UbxTransmit_t)(const uint8_t[], size_t);
typedef void (*UbxDelay_t)(uint32_t);
typedef void (*UbxChangeBaudRate_t)(uint32_t);

TEST(UbloxCommands, test_ubloxInit) {
    auto transmit = [](const uint8_t[], size_t) { return (int8_t)LIBPERIPH_OK; };
    auto delay = [](uint32_t delay_ms) {};
    auto changeBaudRate = [](uint32_t rate) {};

    ASSERT_EQ(LIBPERIPH_OK, ubloxInit(transmit, delay, changeBaudRate));
    ASSERT_EQ(LIBPERIPH_ERROR, ubloxInit(nullptr, delay, changeBaudRate));
    ASSERT_EQ(LIBPERIPH_ERROR, ubloxInit(transmit, nullptr, changeBaudRate));
    ASSERT_EQ(LIBPERIPH_ERROR, ubloxInit(transmit, delay, nullptr));
}

TEST(UbloxCommands, test_ubloxConfigure) {
    auto transmit = [](const uint8_t[], size_t) { return (int8_t)LIBPERIPH_OK; };
    auto delay = [](uint32_t delay_ms) {};
    auto changeBaudRate = [](uint32_t rate) {};

    ASSERT_EQ(LIBPERIPH_OK, ubloxInit(transmit, delay, changeBaudRate));
    ASSERT_EQ(LIBPERIPH_OK, ubloxConfigure(0));

    ASSERT_EQ(LIBPERIPH_ERROR, ubloxInit(NULL, delay, changeBaudRate));
    ASSERT_EQ(LIBPERIPH_ERROR, ubloxConfigure(0));
}

TEST(UbloxCommands, test_ubloxExecuteCommand) {
    auto transmit = [](const uint8_t[], size_t) { return (int8_t)LIBPERIPH_OK; };
    auto delay = [](uint32_t delay_ms) {};
    auto changeBaudRate = [](uint32_t rate) {};

    ASSERT_EQ(LIBPERIPH_OK, ubloxInit(transmit, delay, changeBaudRate));
    ASSERT_EQ(LIBPERIPH_OK, ubloxExecuteCommand(NODE_SET_BAUDRATE_921600));
    ASSERT_EQ(LIBPERIPH_ERROR, ubloxExecuteCommand(UBX_UNKNOWN_COMMAND));
}

TEST(UbloxCommands, test_ubloxGetCommand) {
    ASSERT_EQ(NODE_SET_BAUDRATE_921600, ubloxGetCommand(0));
    ASSERT_EQ(UBX_UNKNOWN_COMMAND, ubloxGetCommand(255));
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
