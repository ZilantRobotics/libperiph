/**
* @file motors.hpp
* @brief Motors class
*/
#ifndef MOTORS_HPP
#define MOTORS_HPP


#include <hal.h>


class Motors
{
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

#endif /* MOTORS_HPP */
