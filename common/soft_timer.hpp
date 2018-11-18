#ifndef __SOFT_TIMER_HPP
#define __SOFT_TIMER_HPP

#include "stdint.h"


/**
* @brief Software stopwatch/timer
*/
class SoftTimer
{
	public:
		enum TimerStatus_t
		{
			CREATED,
			WORKING,
			WAITING,
			STOPPED,
			FINISHED,
		};
		SoftTimer(uint32_t sys_clock);
		void StartUs(uint16_t timeUs);
		void StartMs(uint16_t timeMs);
		void Continue();
		void Wait();
		void Stop();
		TimerStatus_t GetStatus();
		uint32_t GetRestTime();
		uint32_t GetElapsedTime();
	private:
		bool IsTimerEnd() const;
	
		uint8_t  StartOverflows;
		uint8_t  RestOverflows;
		uint8_t  EndOverflows;
		
		uint32_t StartCount;
		uint32_t RestCount;
		uint32_t EndCount;
	
		TimerStatus_t Status;
		uint32_t ONE_MS;
		uint32_t ONE_S;
		uint32_t PERIOD;
};


#endif //__TIMER_HPP
