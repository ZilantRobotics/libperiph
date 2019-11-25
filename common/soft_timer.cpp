/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file soft_timer.cpp
 * @author d.ponomarev
 * @date Jun 24, 2018
 */

#include "soft_timer.hpp"

// implement your own version outside
__attribute__ ((weak)) uint32_t hwTimerGetTicks() {
	return 0;
}

__attribute__ ((weak)) uint32_t hwTimerGetOverflows() {
	return 0;
}


/**
* @brief Constructor of software timer
*/
SoftTimer::SoftTimer(uint32_t sys_clock): Status(CREATED)
{
	PERIOD = 1073741824 / sys_clock * 4;
	ONE_MS = sys_clock/1000;
	ONE_S = sys_clock;
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
		StartCount = hwTimerGetTicks();
		StartOverflows = hwTimerGetOverflows();
	
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
	uint32_t nowCount = hwTimerGetTicks();
	uint8_t NowOverflows = hwTimerGetOverflows();
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
	uint32_t nowCount = hwTimerGetTicks();
	uint32_t nowOverflow = hwTimerGetOverflows();
	if(StartCount > nowCount)
		return (StartCount - nowCount) / ONE_S + (nowOverflow - StartOverflows)*PERIOD;
	return (StartCount + nowCount) / ONE_S + (nowOverflow - StartOverflows)*PERIOD; 
}
