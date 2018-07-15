/**
* @file soft_timer.cpp
* @brief Implementation of software timer
*/

#include <soft_timer.hpp>

Timer2 SoftTimer::HardTimer;

/**
* @brief Constructor of software timer
*/
SoftTimer::SoftTimer(): Status(TIMER_CREATED)
{
	if (HardTimer.Status == Timer2::HARD_TIMER_NOT_INITIALIZED)
	{
		HardTimer.Init();
	}
}


/**
* @brief Run Timer on Time in us
* @param timeUs - time in us
*/
void SoftTimer::StartUs(uint16_t timeUs)
{
	
}


/**
* @brief Run Timer on Time in ms
* @param timeUs - time in ms
*/
void SoftTimer::StartMs(uint16_t timeMs)
{
	if (timeMs != 0)
	{
		StartCount = HardTimer.GetCount();
		StartOverflows = HardTimer.GetOverflowsCount();
	
		EndCount = StartCount + timeMs * Timer2::ONE_MS;
		EndOverflows = StartOverflows + ( (EndCount < StartCount) ? 1 : 0);
	
		Status = TIMER_WORKING;
	}
	else
	{
		Status = TIMER_FINISHED;
	}
}


/**
* @brief Get status of timer
* @return status - timer status
*/
uint8_t SoftTimer::GetStatus()
{
	uint32_t nowCount = HardTimer.GetCount();
	if ( Status == TIMER_WORKING && IsTimerEnd(nowCount) )
    {
        Status = TIMER_FINISHED;
    }
    return Status;
}


/**
* @brief Get info whether the timer has stopped
* @return status - timer status
*/
uint8_t SoftTimer::IsTimerEnd(uint32_t nowCount)
{
	uint8_t NowOverflows = HardTimer.GetOverflowsCount();
    
    if ( (EndOverflows < NowOverflows) || ( (EndOverflows == NowOverflows) && (EndCount <= nowCount) ) )
        return 1;
    return 0;
}
