#ifndef __TIMER_HPP
#define __TIMER_HPP

#include "stm32f3xx.h"
#include "target.hpp"

/**
* @brief Timer2
*/
class Timer2
{
	public:
		enum HardTimerStatus_t
		{
			HARD_TIMER_NOT_INITIALIZED = 0,
			HARD_TIMER_INITIALIZED = 1,
		};
		enum
		{
			PERIOD = 1073741824 / SYSCLOCK * 4,	///< 32-bits Timer period (8MHz <=> 536.871 sec)
			ONE_US = SYSCLOCK/1000000,			///< 32-bits Timer 1 us (8MHz <=> 8 tics) 
			ONE_MS = SYSCLOCK/1000,				///< 32-bits Timer 1 ms (8MHz <=> 8 000 tics)
			ONE_S = SYSCLOCK,					///< 32-bits Timer 1 s  (8MHz <=> 8 000 000 tics)
		};
		Timer2(): Status(HARD_TIMER_NOT_INITIALIZED) {};
		void Init();
		uint32_t GetCount();
		uint8_t GetOverflowsCount();
			
		HardTimerStatus_t Status;  
	private:
		
};


#endif //__TIMER_HPP
