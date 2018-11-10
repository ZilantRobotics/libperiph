/**
* @file soft_timer.cpp
* @brief Software timer implementation
*/

#include "soft_timer.hpp"

Counter SoftTimer::HardTimer;

/**
* @brief Constructor of software timer
*/
SoftTimer::SoftTimer(): Status(CREATED)
{
	if (HardTimer.GetStatus() == HARD_TIMER_NOT_INITIALIZED)
	{
		HardTimer.Init(TIMER_2);
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
	
		EndCount = StartCount + timeMs * ONE_MS;
		EndOverflows = StartOverflows + ( (EndCount < StartCount) ? 1 : 0);
	
		Status = WORKING;
	}
	else
	{
		Status = FINISHED;
	}
}


/**
* @brief Get status of timer
* @return status - timer status
*/
SoftTimer::TimerStatus_t SoftTimer::GetStatus()
{
	if ( Status == WORKING && IsTimerEnd() )
    {
        Status = FINISHED;
    }
    return Status;
}


/**
* @brief Get info whether the timer has stopped
* @return true, if timer is end, in other way false
*/
bool SoftTimer::IsTimerEnd() const
{
	uint32_t nowCount = HardTimer.GetCount();
	uint8_t NowOverflows = HardTimer.GetOverflowsCount();
    if ( (EndOverflows < NowOverflows) || ( (EndOverflows == NowOverflows) && (EndCount <= nowCount) ) )
        return true;
    return false;
}


/**
* @brief Get elapsed time
* @return elapsed time is seconds
*/
uint32_t SoftTimer::GetElapsedTime()
{
	uint32_t nowCount = HardTimer.GetCount();
	uint32_t nowOverflow = HardTimer.GetOverflowsCount();
	if(StartCount > nowCount)
		return (StartCount - nowCount) / ONE_S + (nowOverflow - StartOverflows)*PERIOD;
	return (StartCount + nowCount) / ONE_S + (nowOverflow - StartOverflows)*PERIOD; 
}
