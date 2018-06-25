/**
* @file timer.cpp
* @brief Реализация программного таймера
*/

#include <soft_timer.hpp>

Timer2 SoftTimer::HardTimer;

/**
* @brief Конструктор программного таймера
*/
SoftTimer::SoftTimer(): Status(TIMER_CREATED)
{
	if (HardTimer.Status == Timer2::HARD_TIMER_NOT_INITIALIZED)
	{
		HardTimer.Init();
	}
}


/**
* @brief Запуск таймер на время в мкс
* @param timeUs - время в мкс
*/
void SoftTimer::StartUs(uint16_t timeUs)
{
	
}


/**
* @brief Запуск таймер на время в мс
* @param timeMs - время в мкс
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
* @brief Получить статус таймера
* @return status - статус таймера 
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
* @brief Получить статус таймера
* @return status - статус таймера 
*/
uint8_t SoftTimer::IsTimerEnd(uint32_t nowCount)
{
	uint8_t NowOverflows = HardTimer.GetOverflowsCount();
    
    if ( (EndOverflows < NowOverflows) || ( (EndOverflows == NowOverflows) && (EndCount <= nowCount) ) )
        return 1;
    return 0;
}
