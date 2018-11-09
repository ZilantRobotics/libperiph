#ifndef __TIMER_HPP
#define __TIMER_HPP

#include "target.hpp"
#include "stm32f3xx.h"


enum HardTimerStatus_t
{
	HARD_TIMER_NOT_INITIALIZED,
	HARD_TIMER_INITIALIZED,
};


enum
{
	PERIOD = 1073741824 / SYSCLOCK * 4,	///< 32-bits Timer period (8MHz <=> 536.871 sec)
	ONE_US = SYSCLOCK/1000000,			///< 32-bits Timer 1 us (8MHz <=> 8 tics) 
	ONE_MS = SYSCLOCK/1000,				///< 32-bits Timer 1 ms (8MHz <=> 8 000 tics)
	ONE_S = SYSCLOCK,					///< 32-bits Timer 1 s  (8MHz <=> 8 000 000 tics)
};	


enum HardTimerNumber_t
{
	TIMER_2,
	TIMER_3,
	TIMER_4,
};


/**
* @brief General-purpose timers (TIM2/TIM3/TIM4)
*/
class GeneralPurposeTimers
{
	private:
		HardTimerStatus_t Status; 
	public:
		GeneralPurposeTimers(): Status(HARD_TIMER_NOT_INITIALIZED) {}
		virtual void Init() {}
		HardTimerStatus_t GetStatus() const {return Status;}
		void SetStatus(HardTimerStatus_t status) {Status = status;}
};


/**
* @brief Counter
*/
class Counter : public GeneralPurposeTimers
{
	public:
		Counter() {}
		virtual void Init() override;
		uint32_t GetCount();
		uint8_t GetOverflowsCount();	
};



/**
* @brief Pwm
*/
class Pwm : public GeneralPurposeTimers
{
	public:
		Pwm() {};
		virtual void Init() override;
	
		void SetDutyCycle(uint8_t);
		uint8_t GetDutyCycle();	
	
		void SetFrequency(uint32_t f);
		uint32_t GetFrequency();
};


#endif //__TIMER_HPP
