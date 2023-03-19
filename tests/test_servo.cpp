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

TEST(Servo, map_0_same_output_values) {
    auto map_with_same_output_values = [](auto x) {
        return (PwmDurationMillisecond_t)mapFloat(x, 0, 8191, 1000, 1000);
    };

    ASSERT_EQ(1000, map_with_same_output_values(-1));
    ASSERT_EQ(1000, map_with_same_output_values(0));
    ASSERT_EQ(1000, map_with_same_output_values(4096));
    ASSERT_EQ(1000, map_with_same_output_values(8191));
    ASSERT_EQ(1000, map_with_same_output_values(8192));
}

TEST(Servo, map_1_wrong_input) {
    float x = -1000, in_min = 0, in_max = 8191, out_min = 1000, out_max = 2000;
    ASSERT_EQ(out_min, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));

    x = 10000, in_min = 0, in_max = 8191, out_min = 1000, out_max = 2000;
    ASSERT_EQ(out_max, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));

    x = -1000, in_min = 0, in_max = 8191, out_min = 2000, out_max = 1000;
    ASSERT_EQ(out_min, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));

    x = 10000, in_min = 0, in_max = 8191, out_min = 2000, out_max = 1000;
    ASSERT_EQ(out_max, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));
}

TEST(Servo, map_2_boundaries) {
    float x = 0, in_min = 0, in_max = 8191, out_min = 1000, out_max = 2000;
    ASSERT_EQ(out_min, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));

    x = 8191, in_min = 0, in_max = 8191, out_min = 1000, out_max = 2000;
    ASSERT_EQ(out_max, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));

    x = 0, in_min = 0, in_max = 8191, out_min = 2000, out_max = 1000;
    ASSERT_EQ(out_min, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));

    x = 8191, in_min = 0, in_max = 8191, out_min = 2000, out_max = 1000;
    ASSERT_EQ(out_max, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));
}

TEST(Servo, map_3_intermediate) {
    float x = 4096, in_min = 0, in_max = 8191, out_min = 1000, out_max = 2000;
    ASSERT_EQ(1500, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));

    x = 4096, in_min = 0, in_max = 8191, out_min = 2000, out_max = 1000;
    ASSERT_EQ(1499, (PwmDurationMillisecond_t)mapFloat(x, in_min, in_max, out_min, out_max));
}

TEST(Servo, map_4_direct_pwm_percent) {
    auto calculate_direct_pwm_percent = [](auto x) {
        return (PwmDurationMillisecond_t)mapFloat(x, 1000, 2000, 0, 100);
    };

    ASSERT_EQ(0,   calculate_direct_pwm_percent(999));
    ASSERT_EQ(0,   calculate_direct_pwm_percent(1000));
    ASSERT_EQ(25,  calculate_direct_pwm_percent(1250));
    ASSERT_EQ(50,  calculate_direct_pwm_percent(1500));
    ASSERT_EQ(75,  calculate_direct_pwm_percent(1750));
    ASSERT_EQ(100, calculate_direct_pwm_percent(2000));
    ASSERT_EQ(100, calculate_direct_pwm_percent(2001));
}

TEST(Servo, map_5_inverse_pwm_percent) {
    auto calculate_inverse_pwm_percent = [](auto x) {
        return (PwmDurationMillisecond_t)mapFloat(x, 2000, 1000, 0, 100);
    };

    ASSERT_EQ(100, calculate_inverse_pwm_percent(999));
    ASSERT_EQ(100, calculate_inverse_pwm_percent(1000));
    ASSERT_EQ(75,  calculate_inverse_pwm_percent(1250));
    ASSERT_EQ(50,  calculate_inverse_pwm_percent(1500));
    ASSERT_EQ(25,  calculate_inverse_pwm_percent(1750));
    ASSERT_EQ(0,   calculate_inverse_pwm_percent(2000));
    ASSERT_EQ(0,   calculate_inverse_pwm_percent(2001));
}

TEST(Servo, mapRawCommandToPwm_0_same_output_values) {
    RawCommand_t rc_value;
    PwmDurationMillisecond_t min_pwm = 1000, max_pwm = 1000, def_pwm = 1500;

    rc_value = 4096;
    ASSERT_EQ(1000, mapRawCommandToPwm(rc_value, min_pwm, max_pwm, def_pwm));
}

TEST(Servo, mapRawCommandToPwm_1_wrong_input) {
    RawCommand_t rc_value;
    PwmDurationMillisecond_t min_pwm = 1000, max_pwm = 2000, def_pwm = 1500;

    rc_value = -1;
    ASSERT_EQ(1500, mapRawCommandToPwm(rc_value, min_pwm, max_pwm, def_pwm));

    rc_value = 8192;
    ASSERT_EQ(1500, mapRawCommandToPwm(rc_value, min_pwm, max_pwm, def_pwm));
}

TEST(Servo, mapRawCommandToPwm_2_boundaries) {
    RawCommand_t rc_value;
    PwmDurationMillisecond_t min_pwm = 1000, max_pwm = 2000, def_pwm = 1500;

    rc_value = 0;
    ASSERT_EQ(1000, mapRawCommandToPwm(rc_value, min_pwm, max_pwm, def_pwm));

    rc_value = 8191;
    ASSERT_EQ(2000, mapRawCommandToPwm(rc_value, min_pwm, max_pwm, def_pwm));
}

TEST(Servo, mapRawCommandToPwm_3_intermediate) {
    RawCommand_t rc_value;
    PwmDurationMillisecond_t min_pwm, max_pwm, def_pwm;

    rc_value = 4096;
    min_pwm = 1000, max_pwm = 2000, def_pwm = 1500;
    ASSERT_EQ(1500, mapRawCommandToPwm(rc_value, min_pwm, max_pwm, def_pwm));

    rc_value = 4095;
    min_pwm = 2000, max_pwm = 1000, def_pwm = 1500;
    ASSERT_EQ(1500, mapRawCommandToPwm(rc_value, min_pwm, max_pwm, def_pwm));
}

TEST(Servo, mapRawCommandToPwm_4_real_case) {
    RawCommand_t rc_value = 81;
    PwmDurationMillisecond_t min_pwm = 1700, max_pwm = 1330, def_pwm = 1570;

    ASSERT_EQ(1696, mapRawCommandToPwm(rc_value, min_pwm, max_pwm, def_pwm));
}

int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
