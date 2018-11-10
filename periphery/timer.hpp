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
	// Advanced timers
	TIMER_1,
	TIMER_8,
	
	// General-Purpose timers
	TIMER_2,
	TIMER_3,
	TIMER_4,
	
	// Basic timers
	TIMER_6,
	TIMER_7,
	
	// General-purpose timers
	TIMER_15,
	TIMER_16,
	TIMER_17,
};

/**
* @brief General-purpose timers (TIM2/TIM3/TIM4)
*/
class GeneralPurposeTimers
{
	private:
		HardTimerStatus_t Status; 
	protected:
		enum
		{
			TIMER_AMOUNT = 10,
		};
		static bool IsTimerUsed[TIMER_AMOUNT];
		static TIM_TypeDef* HardTimerTable[TIMER_AMOUNT];
		TIM_TypeDef* Timer;
	public:
		GeneralPurposeTimers();
		virtual void Init(HardTimerNumber_t hardTimerNumber) {}
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
		virtual void Init(HardTimerNumber_t hardTimerNumber) override;
		uint32_t GetCount();
		uint8_t GetOverflowsCount();	
};



/**
* @brief Pwm
*/
class Pwm : public GeneralPurposeTimers
{
	public:
		enum PwmChannel_t
		{
			NO_CHANNEL,
			CHANNEL_1,
			CHANNEL_2,
			CHANNEL_3,
			CHANNEL_4,
		};
		Pwm();
		virtual void Init(HardTimerNumber_t hardTimerNumber) override;
	
		void SetDutyCycle(uint8_t);
		uint8_t GetDutyCycle();	
	
		void SetFrequency(uint32_t f);
		uint32_t GetFrequency();
			
		void SetChannel(PwmChannel_t channel);
	private:
		PwmChannel_t ChannelNumber;
};


#endif //__TIMER_HPP
