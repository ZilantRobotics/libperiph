#ifndef __TIMER_HPP
#define __TIMER_HPP

#include "stm32f3xx.h"
#include "target.hpp"

/**
* @brief Timer2
*/
struct Timer2
{
	enum HardTimerStatus_t
	{
		HARD_TIMER_NOT_INITIALIZED = 0,
		HARD_TIMER_INITIALIZED = 1,
	};
	enum
	{
		PERIOD = 1073741824 / SYSCLOCK * 4,	///< (2^32 / SYSCLOCK) = 536.871 sec
		ONE_US = SYSCLOCK/1000000,			///< 1 us = 8 tack's
		ONE_MS = SYSCLOCK/1000,				///< 1 ms = 8000 tack's
		ONE_S = SYSCLOCK,					///< 1 sec = 8 000 000 tact's
	};
	Timer2(): Status(HARD_TIMER_NOT_INITIALIZED) {};
	void Init();
	uint32_t GetCount();
	uint8_t GetOverflowsCount();
        
	HardTimerStatus_t Status;  
};


#endif //__TIMER_HPP
