/**
* @file timer.cpp
* @brief Implementation of work with timers
*/

#include <timer.hpp>
#include <target.hpp>

uint8_t OverflowsCount = 0;	///< crutch: interrupt like C

/**
* @brief Init timer
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
* @brief Get value of counter register
* @return value of count register
*/
uint32_t Timer2::GetCount()
{
    return TIM2->CNT;
}


/**
* @brief Get number of timer overflows
* @return OverflowCount - number of timer overflows
*/
uint8_t Timer2::GetOverflowsCount()
{
    return OverflowsCount;
}


/**
* @brief TIM2 interrupt handler
*/
extern "C"
{
	void TIM2_IRQHandler()
	{
		TIM2->SR = 0;
		OverflowsCount++;
	}
}
