/**
* @file timer.cpp
* @brief Implementation of work with timers
*/

#include "timer.hpp"
#include "target.hpp"

uint8_t OverflowsCount = 0;	///< crutch: interrupt like C


/**
* @brief Init counter
*/
void Counter::Init()
{
	if (GetStatus() == HARD_TIMER_NOT_INITIALIZED)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Clocking
		TIM2->CR1 = 0x01;					// Counter enabled
        TIM2->DIER = TIM_DIER_TIE;          // Trigger interrupt enabled
		TIM2->DIER |= TIM_DIER_UIE;         // Update interrupt enabled
		NVIC_EnableIRQ(TIM2_IRQn);			// Enable interrupt
		SetStatus(HARD_TIMER_INITIALIZED);	
	}
}


/**
* @brief Get value of counter register
* @return value of count register
*/
uint32_t Counter::GetCount()
{
    return TIM2->CNT;
}


/**
* @brief Get number of timer overflows
* @return OverflowCount - number of timer overflows
*/
uint8_t Counter::GetOverflowsCount()
{
    return OverflowsCount;
}


/**
* @brief Init pwm
*/
void Pwm::Init()
{
	if (GetStatus() == HARD_TIMER_NOT_INITIALIZED)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;							// Clocking
		SetFrequency(0xFFFF);
		SetDutyCycle(100);
		TIM3->CCMR1 = (7 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;  // Set PWM mode
		TIM3->CCER = TIM_CCER_CC1E;									// Capture/Compare 1 output enable
		TIM3->EGR = TIM_EGR_UG;										// Update generation
		TIM3->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;						// Counter enabled
	}
}


/**
* @brief Set frequency
* @param[in] frequency
*/
void Pwm::SetFrequency(uint32_t f)
{
	TIM3->ARR = f;
}


/**
* @brief Set duty cycle
* @param[in] duty cycle in range [0; 100]
*/
void Pwm::SetDutyCycle(uint8_t d)
{
	if (d <= 100)
		TIM3->CCR1 = TIM3->ARR/100*(100-d);
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
