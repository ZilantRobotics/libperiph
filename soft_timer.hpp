#ifndef __SOFT_TIMER_HPP
#define __SOFT_TIMER_HPP

#include "stm32f3xx.h"
#include <timer.hpp>

enum TimerStatus_t
{
	TIMER_CREATED = 0,
	TIMER_WORKING = 1,
	TIMER_WAITING = 2,
	TIMER_STOPPED = 3,
	TIMER_FINISHED = 4,
};

/**
* @brief Программный таймер
*/
class SoftTimer
{
	public:
		SoftTimer();
		void StartUs(uint16_t timeUs);
		void StartMs(uint16_t timeMs);
		void Continue();
		void Wait();
		void Stop();
		uint8_t GetStatus();
		uint32_t GetRestTime();
		uint32_t GetElapsedTime();
	private:
		TimerStatus_t  Status;
		
		uint8_t  StartOverflows;
		uint8_t  RestOverflows;
		uint8_t  EndOverflows;
		
		uint32_t StartCount;
		uint32_t RestCount;
		uint32_t EndCount;
	
		static Timer2 HardTimer;
	
		uint8_t IsTimerEnd(uint32_t nowCount);
};


#endif //__TIMER_HPP
