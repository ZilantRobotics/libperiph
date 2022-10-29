/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file encoder.cpp
 * @author d.ponomarev
 * @brief Encoder implementation
 * @date Oct 20, 2018
 */

#include "encoder.hpp"


static void left_wheel_a_cb();
static void right_wheel_a_cb();
static void left_wheel_b_cb();
static void right_wheel_b_cb();
static void speed_tmr_cb();


static int32_t LeftEncoderTicks = 0;
static int32_t RightEncoderTicks = 0;
static int32_t RightEncoderTicksCash = 0;
static int32_t LeftEncoderTicksCash = 0;
static float LeftEncoderSpeed = 0;
static float RightEncoderSpeed = 0;
static float TimerCallbacksPerSecond;

// implement your own version from the outside
__attribute__ ((weak)) bool getEncoderPin(EncoderPin encoder_pin) {
    return false;
}


void Encoder::Reset()
{
    RightEncoderTicks = 0;
    LeftEncoderTicks = 0;

    RightEncoderTicksCash = 0;
    LeftEncoderTicksCash = 0;
}


int32_t Encoder::GetLeftValue()
{
    return LeftEncoderTicks;
}


int32_t Encoder::GetRightValue()
{
    return RightEncoderTicks;
}


int32_t Encoder::GetLeftSpeed()
{
    return LeftEncoderSpeed;
}


int32_t Encoder::GetRightSpeed()
{
    return RightEncoderSpeed;
}


/**
* @brief Calculate and update value of encoders speed static variable
**/
static void speed_tmr_cb()
{
    float right_delta = RightEncoderTicks - RightEncoderTicksCash;
    RightEncoderTicksCash = RightEncoderTicks;
    RightEncoderSpeed = right_delta * TimerCallbacksPerSecond;

    float left_delta = LeftEncoderTicks - LeftEncoderTicksCash;
    LeftEncoderTicksCash = LeftEncoderTicks;
    LeftEncoderSpeed = left_delta * TimerCallbacksPerSecond;
}


/**
* @brief Increase or decrease value of encoder counter when interrupt occur
**/
static void left_wheel_a_cb()
{
    if (getEncoderPin(LEFT_ENC_A_CH)) {
        if (getEncoderPin(LEFT_ENC_B_CH))
            LeftEncoderTicks++;
        else
            LeftEncoderTicks--;
    } else {
        if (getEncoderPin(LEFT_ENC_B_CH))
            LeftEncoderTicks--;
        else
            LeftEncoderTicks++;
    }
}


/**
* @brief Increase or decrease value of encoder counter when interrupt occur
**/
static void right_wheel_a_cb()
{
    if (getEncoderPin(RIGHT_ENC_A_CH)) {
        if (getEncoderPin(RIGHT_ENC_B_CH))
            RightEncoderTicks++;
        else
            RightEncoderTicks--;
    } else {
        if (getEncoderPin(RIGHT_ENC_B_CH))
            RightEncoderTicks--;
        else
            RightEncoderTicks++;
    }
}


/**
* @brief Increase or decrease value of encoder counter when interrupt occur
**/
static void left_wheel_b_cb()
{
    if (getEncoderPin(LEFT_ENC_B_CH)) {
        if (getEncoderPin(LEFT_ENC_A_CH))
            LeftEncoderTicks--;
        else
            LeftEncoderTicks++;
    } else {
        if (getEncoderPin(LEFT_ENC_A_CH))
            LeftEncoderTicks++;
        else
            LeftEncoderTicks--;
    }
}


/**
* @brief Increase or decrease value of encoder counter when interrupt occur
**/
static void right_wheel_b_cb()
{
    if (getEncoderPin(RIGHT_ENC_B_CH)) {
        if (getEncoderPin(RIGHT_ENC_A_CH))
            RightEncoderTicks--;
        else
            RightEncoderTicks++;
    } else {
        if (getEncoderPin(RIGHT_ENC_A_CH))
            RightEncoderTicks++;
        else
            RightEncoderTicks--;
    }
}
