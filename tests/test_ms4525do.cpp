/**
 * @file test_timer_capture.cpp
 * @author d.ponomarev
 * @date Jun 05, 2021
 */

#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
#include "ms4525do.h"


#define I2C_RESPONSE_SIZE   4


/**
 * @brief MS4525DO. Page 5. Sensor Output at Significant Percentages
 */
void fill_pressure(uint16_t table_value) {
    uint8_t ms4525do_rx_buf[I2C_RESPONSE_SIZE];
    uint16_t raw = table_value;
    ms4525do_rx_buf[0] = (0x3F & (raw >> 8));
    ms4525do_rx_buf[1] = 0xFF & raw;
    ms4525doFillBuffer(ms4525do_rx_buf);
}
TEST(ms4525do, pressure_less_then_min) {
    float raw_temperature, raw_diff_press;
    float expected_pressure = 8618.45;

    fill_pressure(0x0000);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_TRUE(abs(raw_diff_press - expected_pressure) < 2.0);
}
TEST(ms4525do, pressure_min) {
    float raw_temperature, raw_diff_press;
    float expected_pressure = 6894.757f;

    fill_pressure(0x0666);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_TRUE(abs(raw_diff_press - expected_pressure) < 2.0);

}
TEST(ms4525do, pressure_zero) {
    float raw_temperature, raw_diff_press;
    float expected_pressure = 0;

    fill_pressure(0x2000);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_TRUE(abs(raw_diff_press - expected_pressure) < 2.0);
}
TEST(ms4525do, pressure_max) {
    float raw_temperature, raw_diff_press;
    float expected_pressure = -6894.757f;

    fill_pressure(0x399A);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_TRUE(abs(raw_diff_press - expected_pressure) < 2.0);

}
TEST(ms4525do, pressure_more_then_max) {
    float raw_temperature, raw_diff_press;
    float expected_pressure = -8618.45;

    fill_pressure(0x3FFF);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_EQ(round(raw_diff_press), round(expected_pressure));
}


/**
 * @brief MS4525DO. Page 5. Temperature Output vs Counts
 */
void fill_temperature(uint16_t table_value) {
    uint8_t ms4525do_rx_buf[I2C_RESPONSE_SIZE];
    uint16_t raw = table_value;
    ms4525do_rx_buf[2] = (0xFF & (raw >> 3));
    ms4525do_rx_buf[3] = (0x07 & raw) << 5;
    ms4525doFillBuffer(ms4525do_rx_buf);
}
TEST(ms4525do, temperature_minus_50) {
    float raw_temperature, raw_diff_press;
    float expected_temperature = -50;

    fill_temperature(0x0000);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_EQ(raw_temperature, expected_temperature);
}
TEST(ms4525do, temperature_zero) {
    float raw_temperature, raw_diff_press;
    float expected_temperature = 0;

    fill_temperature(0x01FF);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_EQ(round(raw_temperature), expected_temperature);
}
TEST(ms4525do, temperature_plus_10) {
    float raw_temperature, raw_diff_press;
    float expected_temperature = 10;

    fill_temperature(0x0266);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_EQ(round(raw_temperature), expected_temperature);
}
TEST(ms4525do, temperature_plus_85) {
    float raw_temperature, raw_diff_press;
    float expected_temperature = 85;

    fill_temperature(0x0565);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_EQ(round(raw_temperature), expected_temperature);
}
TEST(ms4525do, temperature_plus_150) {
    float raw_temperature, raw_diff_press;
    float expected_temperature = 150;

    fill_temperature(0x07FF);
    ms4525doParse(&raw_temperature, &raw_diff_press);

    ASSERT_EQ(raw_temperature, expected_temperature);
}


int main (int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}