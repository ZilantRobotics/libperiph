/*
 * Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <gtest/gtest.h>
#include "hal_adc.h"
#include "libperiph_common.h"

uint16_t ubuntu_adc[MAX_CHANNELS_NUM] = {
    365,    ///< 5.0 / (3.3 * 17.0 / 4095.0)
    3102,   ///< 5.0 / (3.3 * 2.0 / 4095.0)
    400,    ///< 400
    1750,   ///< 1750
};


TEST(hal_adc, test_adcInitDma) {
    ASSERT_EQ(LIBPERIPH_OK, adcInitDma(4));
    ASSERT_EQ(LIBPERIPH_ERROR, adcInitDma(MAX_CHANNELS_NUM));
}

TEST(hal_adc, test_adcGetAll) {
    ASSERT_EQ(LIBPERIPH_OK, adcInitDma(4));
    uint16_t adc_value[4];
    ASSERT_EQ(4, adcGetAll(adc_value));

    ASSERT_EQ(LIBPERIPH_ERROR, adcGetAll(NULL));
}

TEST(hal_adc, test_adcGet) {
    ASSERT_EQ(LIBPERIPH_OK, adcInitDma(4));

    ASSERT_EQ(ubuntu_adc[0], adcGet(0));
    ASSERT_EQ(ubuntu_adc[1], adcGet(1));
    ASSERT_EQ(ubuntu_adc[2], adcGet(2));
    ASSERT_EQ(ubuntu_adc[3], adcGet(3));
    ASSERT_EQ(65535, adcGet(4));

}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
