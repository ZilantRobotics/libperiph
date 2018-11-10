/**
* @file motor.cpp
* @brief Motor work implementation
*/

#include "motor.hpp"
#include "timer.hpp"


Motor::Motor()
{
	
}

/**
* @brief Init motor
*/
void Motor::Init()
{
	Power.Init(TIMER_3);
	Power.SetChannel(Pwm::CHANNEL_1);
	Power.SetDutyCycle(50);
}


/**
* @brief Set power
* @param[in] power in range [0; 100]
*/
void Motor::SetPower(uint8_t power)
{
	Power.SetDutyCycle(power);
}


/**
* @brief Set direction
* @param[in] direction - FORWARD or BACKWARD
*/
void Motor::SetDirection(MotorDirection_t direction)
{
	if(direction == FORWARD)
	{
		Power.SetChannel(Pwm::CHANNEL_1);
	}
	else if(direction == BACKWARD)
	{
		Power.SetChannel(Pwm::CHANNEL_2);
	}
	else
	{
		return;
	}
	Direction = direction;
}


/**
* @brief Stop motor
*/
void Motor::Stop()
{
	Power.SetDutyCycle(0);
}
