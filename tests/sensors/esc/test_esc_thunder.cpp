/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "esc/thunder.h"

static uint8_t spd_3000_buf[] = "SPD:0BB8 ";
static uint8_t tmos_100_buf[] = "TMOS:0064 ";
static uint8_t tmot_27_buf[] = "TMOT:001B ";
static uint8_t curi_10_amper_buf[] = "CURI:03E8 ";

static uint8_t volt_50_volts_buf[] = "VOLT:1388 ";
static uint8_t volt_60_3_volts_buf[] = "VOLT:178E ";
static uint8_t volt_105_volts_buf[] = "VOLT:2904 ";
static uint8_t volt_wrong_buf[] = "VOLT:29G4 ";

void fill_ring_buffer(EscThunderFeedback& esc_thunder, const uint8_t* buf, uint8_t size) {
    for (size_t idx = 0; idx < size; idx++) {
        thunderNextByte(&esc_thunder, buf[idx]);
    }
}

TEST(ESCThunder, test_hexByteToUint8) {
    ASSERT_EQ(0, hexByteToUint8('0'));
    ASSERT_EQ(9, hexByteToUint8('9'));
    ASSERT_EQ(10, hexByteToUint8('A'));
    ASSERT_EQ(15, hexByteToUint8('F'));
}

TEST(ESCThunder, test_hexArrayToUint32) {
    ASSERT_EQ(6030, hexArrayToUint32((const uint8_t*)"178E", 4));
    ASSERT_EQ(6030, hexArrayToUint32((const uint8_t*)"0000178E", 8));
}

TEST(ESCThunder, test_thunderParseMessageInRingBuffer_spd) {
    EscThunderFeedback esc_thunder;
    thunderInit(&esc_thunder);

    fill_ring_buffer(esc_thunder, spd_3000_buf, sizeof(spd_3000_buf) - 1);
    ASSERT_EQ(ESC_THUNDER_SPD, thunderParseMessageInRingBuffer(&esc_thunder));
    EXPECT_EQ(3000, esc_thunder.spd);
}


TEST(ESCThunder, test_thunderParseMessageInRingBuffer_tmos) {
    EscThunderFeedback esc_thunder;
    thunderInit(&esc_thunder);

    fill_ring_buffer(esc_thunder, tmos_100_buf, sizeof(tmos_100_buf) - 1);
    ASSERT_EQ(ESC_THUNDER_TMOS, thunderParseMessageInRingBuffer(&esc_thunder));
    EXPECT_EQ(100, esc_thunder.tmos);
}

TEST(ESCThunder, test_thunderParseMessageInRingBuffer_tmot) {
    EscThunderFeedback esc_thunder;
    thunderInit(&esc_thunder);

    fill_ring_buffer(esc_thunder, tmot_27_buf, sizeof(tmot_27_buf) - 1);
    ASSERT_EQ(ESC_THUNDER_TMOT, thunderParseMessageInRingBuffer(&esc_thunder));
    EXPECT_EQ(27, esc_thunder.tmot);
}

TEST(ESCThunder, test_thunderParseMessageInRingBuffer_curi) {
    EscThunderFeedback esc_thunder;
    thunderInit(&esc_thunder);

    fill_ring_buffer(esc_thunder, curi_10_amper_buf, sizeof(curi_10_amper_buf) - 1);
    ASSERT_EQ(ESC_THUNDER_CURI, thunderParseMessageInRingBuffer(&esc_thunder));
    EXPECT_NEAR(10.0, esc_thunder.curi, 0.01);
}

TEST(ESCThunder, test_thunderParseMessageInRingBuffer_volt) {
    EscThunderFeedback esc_thunder;
    thunderInit(&esc_thunder);

    fill_ring_buffer(esc_thunder, volt_50_volts_buf, sizeof(volt_50_volts_buf) - 1);
    ASSERT_EQ(ESC_THUNDER_VOLT, thunderParseMessageInRingBuffer(&esc_thunder));
    EXPECT_NEAR(50.0, esc_thunder.volt, 0.01);

    fill_ring_buffer(esc_thunder, volt_60_3_volts_buf, sizeof(volt_60_3_volts_buf) - 1);
    ASSERT_EQ(ESC_THUNDER_VOLT, thunderParseMessageInRingBuffer(&esc_thunder));
    EXPECT_NEAR(60.3, esc_thunder.volt, 0.01);

    fill_ring_buffer(esc_thunder, volt_105_volts_buf, sizeof(volt_105_volts_buf) - 1);
    ASSERT_EQ(ESC_THUNDER_VOLT, thunderParseMessageInRingBuffer(&esc_thunder));
    EXPECT_NEAR(105.0, esc_thunder.volt, 0.01);
}


TEST(ESCThunder, test_thunderNextByte_voltage) {
    EscThunderFeedback esc_thunder;
    thunderInit(&esc_thunder);

    for (uint8_t idx = 0; idx < sizeof(volt_60_3_volts_buf) - 2; idx++) {
        ASSERT_EQ(ESC_THUNDER_UNKNOWN, thunderNextByte(&esc_thunder, volt_60_3_volts_buf[idx]));
    }
    auto res = thunderNextByte(&esc_thunder, volt_60_3_volts_buf[sizeof(volt_60_3_volts_buf) - 2]);
    ASSERT_EQ(ESC_THUNDER_VOLT, res);
    EXPECT_NEAR(60.3, esc_thunder.volt, 0.01);
}

TEST(ESCThunder, test_mull_args) {
    EscThunderFeedback esc_thunder;
    thunderInit(NULL);

    thunderNextByte(&esc_thunder, 0);

    thunderInit(&esc_thunder);
}

TEST(ESCThunder, test_wrong_messages) {
    EscThunderFeedback esc_thunder;
    thunderInit(&esc_thunder);

    for (uint8_t idx = 0; idx < sizeof(volt_wrong_buf) - 2; idx++) {
        ASSERT_EQ(ESC_THUNDER_UNKNOWN, thunderNextByte(&esc_thunder, volt_wrong_buf[idx]));
    }
    auto res = thunderNextByte(&esc_thunder, volt_wrong_buf[sizeof(volt_wrong_buf) - 2]);
    ASSERT_EQ(ESC_THUNDER_UNKNOWN, res);
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
