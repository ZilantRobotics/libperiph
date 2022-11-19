/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file motors.hpp
 * @author d.ponomarev
 * @brief Motors implementation
 * @date Nov 11, 2019
 */

#ifndef MOTORS_HPP_
#define MOTORS_HPP_


#include <hal.h>


class Motors {
public:
    static void Init();
    static void SetLeftPower(int8_t);
    static void SetRightPower(int8_t);
    static int8_t GetLeftPower();
    static int8_t GetRightPower();

private:
    static PWMDriver* PwmDriverLeft;
    static PWMDriver* PwmDriverRight;

    static int32_t PowerToPwm;
    static int8_t MotorLeftDutyCycle;
    static int8_t MotorRightDutyCycle;
};

#endif  // MOTORS_HPP
