#ifndef __SOFT_TIMER_HPP
#define __SOFT_TIMER_HPP

#include "timer.hpp"
#include "stm32f3xx.h"


/**
* @brief Software stopwatch/timer
*/
class SoftTimer
{
	public:
		enum TimerStatus_t
		{
			CREATED,				///< Timer is created, but is not working
			WORKING,				///< Timer is working
			WAITING,				///< Timer is waiting
			STOPPED,				///< Timer is stopped
			FINISHED,				///< Timer is finished
		};
		SoftTimer();							/// Constructor
		void StartUs(uint16_t timeUs);			/// Start timer in us
		void StartMs(uint16_t timeMs);			/// Start timer in üs
		void Continue();						/// Continue timer
		void Wait();							/// Wait timer (u can continue it)
		void Stop();							/// Stop timer (u can't continue it)
		TimerStatus_t GetStatus();				/// Get status
		uint32_t GetRestTime();					/// Get rest time
		uint32_t GetElapsedTime();				/// Get elapsed time
	private:
		bool IsTimerEnd() const;				/// Is Timer End
	
		uint8_t  StartOverflows;				///< Start Overflows
		uint8_t  RestOverflows;					///< Rest Overflows
		uint8_t  EndOverflows;					///< End Overflows
		
		uint32_t StartCount;					///< Start Count
		uint32_t RestCount;						///< Rest Count
		uint32_t EndCount;						///< End Count
	
		TimerStatus_t Status;					///< Status
		static Counter HardTimer;				///< Hard Timer			
};


#endif //__TIMER_HPP
