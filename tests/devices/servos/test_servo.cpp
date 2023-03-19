/**
 * @file test_servo.cpp
 * @author d.ponomarev
 * @date Aug 11, 2022
 */

#include <gtest/gtest.h>
#include <iostream>
#include "servos.h"


TEST(Servo, test_init_channel) {
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };

    ASSERT_EQ(0, servosInitChannel((Channel_t)0, &servo_params));
    ASSERT_EQ(0, servosInitChannel((Channel_t)0, &servo_params));

    // bad arguments
    ASSERT_EQ(-1, servosInitChannel((Channel_t)20, &servo_params));
    ASSERT_EQ(-1, servosInitChannel((Channel_t)0, nullptr));
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
    servosUpdateParams((Channel_t)0, nullptr);
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

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
