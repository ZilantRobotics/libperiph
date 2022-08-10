/**
 * @file test_servo.cpp
 * @author d.ponomarev
 * @date Aug 11, 2022
 */

#include <gtest/gtest.h>
#include <iostream>
#include "servos.h"


TEST(Servo, test_init) {
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };

    ASSERT_EQ(0, uavcanServosInitChannel((Channel_t)0, &servo_params));
    ASSERT_EQ(0, uavcanServosInitChannel((Channel_t)0, &servo_params));

    // bad arguments
    ASSERT_EQ(-1, uavcanServosInitChannel((Channel_t)20, &servo_params));
    ASSERT_EQ(-1, uavcanServosInitChannel((Channel_t)0, nullptr));
}

TEST(Servo, test_update_params) {
    ServoParameters_t servo_params = {
        .ch = 0,
        .min = 1000,
        .max = 2000,
        .def = 1000
    };

    // bad arguments
    uavcanServosUpdateParams((Channel_t)20, &servo_params);
    uavcanServosUpdateParams((Channel_t)0, nullptr);
}

TEST(Algorithms, map_0_less_than_in_min) {
    uint32_t x = 0, in_min = 2, in_max = 8191, out_min = 1000, out_max = 2000, out;
    out = mapU32(x, in_min, in_max, out_min, out_max);

    // Anyway it is incorrect behaviour below:
    ASSERT_EQ(out, 525479);
}
TEST(Algorithms, map_1_in_min) {
    uint32_t x = 0, in_min = 0, in_max = 8191, out_min = 1000, out_max = 2000, out;
    out = mapU32(x, in_min, in_max, out_min, out_max);
    ASSERT_EQ(out, out_min);
}
TEST(Algorithms, map_2_middle) {
    uint32_t x = 4096, in_min = 0, in_max = 8191, out_min = 1000, out_max = 2000, out;
    out = mapU32(x, in_min, in_max, out_min, out_max);
    ASSERT_EQ(out, 1500);
}
TEST(Algorithms, map_3_in_max) {
    uint32_t x = 8191, in_min = 0, in_max = 8191, out_min = 1000, out_max = 2000, out;
    out = mapU32(x, in_min, in_max, out_min, out_max);
    ASSERT_EQ(out, out_max);
}
TEST(Algorithms, map_4_more_than_in_max) {
    uint32_t x = 8192, in_min = 0, in_max = 8191, out_min = 1000, out_max = 2000, out;
    out = mapU32(x, in_min, in_max, out_min, out_max);
    ASSERT_EQ(out, out_max);
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
