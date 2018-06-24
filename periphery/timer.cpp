/**
* @file timer.cpp
* @brief Реализация таймера
*/

#include <timer.hpp>
#include <target.hpp>
extern TargetBase Target;

uint8_t OverflowsCount = 0;

/**
* @brief Инициализация таймера
*/
void Timer2::Init()
{
	if (Status == HARD_TIMER_NOT_INITIALIZED)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Clocking
		TIM2->CR1 = 0x01;					// Counter enabled
        TIM2->DIER = TIM_DIER_TIE;          // Trigger interrupt enabled
		TIM2->DIER |= TIM_DIER_UIE;         // Update interrupt enabled
		//TIM2->ARR = 0x0000FFFF;          	// Auto-reload value
		NVIC_EnableIRQ(TIM2_IRQn);			// Enable interrupt
		Status = HARD_TIMER_INITIALIZED;	
	}
}


/**
* @brief Получить по ссылке значение счетчика и кол-во прерываний
* @param count - ссылка на переменную, храняющую значение счетчика
* @param interruptCount - ссылка на кол-во прерываний
*/
uint32_t Timer2::GetCount()
{
    return TIM2->CNT;
}


/**
* @brief Получить значение кол-ва переполнений таймера
* @return OverflowCount - кол-во переполнений таймера
*/
uint8_t Timer2::GetOverflowsCount()
{
    return OverflowsCount;
}


/**
* @brief Прерывание таймера2 по переполнению
*/
extern "C"
{
	void TIM2_IRQHandler()
	{
		TIM2->SR = 0;
		OverflowsCount++;
	}
}
