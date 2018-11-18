/**
* @file motor.hpp
* @brief Motor class
*/

#ifndef __MOTOR_HPP
#define __MOTOR_HPP

#include "timer.hpp"


/**
* @brief Work with motors
*/
class Motor
{
	public:
		enum MotorDirection_t
		{
			FORWARD,
			BACKWARD,
		};
		Motor();
		void Init();
		void SetPower(uint8_t power);
		void SetDirection(MotorDirection_t direction);
		void Stop();
	private:
		Pwm Power;
		MotorDirection_t Direction;
};


#endif	// __MOTOR_HPP
