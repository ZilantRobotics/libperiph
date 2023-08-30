/**
 * @file test_servo.cpp
 * @author d.ponomarev
 * @date Aug 11, 2022
 */

#include <gtest/gtest.h>
#include <iostream>
#include "servos.h"


TEST(Servo, test_servosInitChannel) {
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };

    ASSERT_EQ(LIBPERIPH_OK, servosInitChannel(PIN_PB7_TIM4_CH2, &servo_params));
    ASSERT_EQ(LIBPERIPH_OK, servosInitChannel(PIN_PB7_TIM4_CH2, &servo_params));

    // bad arguments
    ASSERT_EQ(LIBPERIPH_ERROR, servosInitChannel((Channel_t)20, &servo_params));
    ASSERT_EQ(LIBPERIPH_ERROR, servosInitChannel(PIN_PB7_TIM4_CH2, nullptr));
}

TEST(Servo, test_update_params) {
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };

    // bad arguments
    servosUpdateParams((Channel_t)20, &servo_params);
    servosUpdateParams(PIN_PB7_TIM4_CH2, nullptr);
}

TEST(Servo, test_set_ttl) {
    servosSetTimeout(0);
}

TEST(Servo, test_servosSetArmingState) {
    servosSetArmingState(false, 100);
    ASSERT_EQ(false, servosGetArmingState(100));

    servosSetArmingState(true, 100);
    ASSERT_EQ(true, servosGetArmingState(100));
}

TEST(Servo, test_servosSetSetpoint) {
    servosSetSetpoint(0, 1000, 100);
    ASSERT_EQ(1000, servosGetSetpoint(0));

    servosSetSetpoint(0, 2000, 100);
    ASSERT_EQ(2000, servosGetSetpoint(0));
}

TEST(Servo, test_servosApplyPwm) {
    servosApplyPwm(1000);
}

TEST(Servo, test_servosIsChannelInited) {
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };

    ASSERT_EQ(LIBPERIPH_OK, servosInitChannel(PIN_PB7_TIM4_CH2, &servo_params));
    ASSERT_TRUE(servosIsChannelInited(PIN_PB7_TIM4_CH2));

    // Bad arguments
    ASSERT_TRUE(servosIsChannelInited(PIN_PB7_TIM4_CH2));
}

TEST(Servo, test_servosGetPwmPercent) {
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };
    ASSERT_EQ(LIBPERIPH_OK, servosInitChannel(PIN_PB7_TIM4_CH2, &servo_params));

    // Default
    ASSERT_EQ(0, servosGetPwmPercent(PIN_PB7_TIM4_CH2));

    // Bad argument
    ASSERT_EQ(-1, servosGetPwmPercent(TIM_CH_AMOUNT));      ///< not existed channel
    ASSERT_EQ(-1, servosGetPwmPercent(PIN_PB6_TIM4_CH1));   ///< not configured channel
}

TEST(Servo, test_servosGetTimerSetpointIndex) {
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };
    ASSERT_EQ(LIBPERIPH_OK, servosInitChannel(PIN_PB7_TIM4_CH2, &servo_params));

    ASSERT_EQ(0, servosGetTimerSetpointIndex(PIN_PB7_TIM4_CH2));
}

TEST(Servo, test_servosGetTimerChannelBySetpointChannel) {
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };
    ASSERT_EQ(LIBPERIPH_OK, servosInitChannel(PIN_PB7_TIM4_CH2, &servo_params));

    ASSERT_EQ(PIN_PB7_TIM4_CH2, servosGetTimerChannelBySetpointChannel(0));
    ASSERT_EQ(TIM_CH_AMOUNT, servosGetTimerChannelBySetpointChannel(1));
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
