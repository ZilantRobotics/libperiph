#ifndef __SOFT_TIMER_HPP
#define __SOFT_TIMER_HPP

#include "stm32f3xx.h"
#include <timer.hpp>

enum TimerStatus_t
{
	TIMER_CREATED = 0,	///< Timer is created, but is not working
	TIMER_WORKING = 1,	///< Timer is working
	TIMER_WAITING = 2,	///< Timer is waiting
	TIMER_STOPPED = 3,	///< Timer is stopped
	TIMER_FINISHED = 4,	///< Timer is finished
};

/**
* @brief Software timer
*/
class SoftTimer
{
	public:
		/// Main methods:
		SoftTimer();							/// Constructor
		void StartUs(uint16_t timeUs);			/// Start timer in us
		void StartMs(uint16_t timeMs);			/// Start timer in us
		void Continue();						/// Continue timer
		void Wait();							/// Wait timer (u can continue it)
		void Stop();							/// Stop timer (u can't continue it)
		uint8_t GetStatus();					/// Get status
		uint32_t GetRestTime();					/// Get rest time
		uint32_t GetElapsedTime();				/// Get elapsed time
	private:
		uint8_t  StartOverflows;				///< Start Overflows
		uint8_t  RestOverflows;					///< Rest Overflows
		uint8_t  EndOverflows;					///< End Overflows
		
		uint32_t StartCount;					///< Start Count
		uint32_t RestCount;						///< Rest Count
		uint32_t EndCount;						///< End Count
	
		TimerStatus_t Status;					///< Status
		static Timer2 HardTimer;				///< Hard Timer			
		
		uint8_t IsTimerEnd(uint32_t nowCount);	/// Is Timer End
};


#endif //__TIMER_HPP
