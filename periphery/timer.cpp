/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file timer.cpp
 * @author d.ponomarev
 * @brief Implementation of work with timers
 * @date Jun 24, 2018
 */

#include "timer.hpp"
#include "target.hpp"
#include <string.h>


uint8_t OverflowsCount = 0;	///< crutch: interrupt like C


/**
* @brief Hard Timer Pointer Table
*/
TIM_TypeDef* GeneralPurposeTimers::HardTimerTable[TIMER_AMOUNT] = 
{
	// Advanced timers
	TIM1,
	TIM8,
	
	// General-Purpose timers
	TIM2,
	TIM3,
	TIM4,
	
	// Basic timers
	TIM6,
	TIM7,
	
	// General-purpose timers
	TIM15,
	TIM16,
	TIM17,
};


/**
* @brief Hard Timer Pointer Table
*/
bool GeneralPurposeTimers::IsTimerUsed[TIMER_AMOUNT] = {0};


/**
* @brief Init GeneralPurposeTimers
*/
GeneralPurposeTimers::GeneralPurposeTimers(): Status(HARD_TIMER_NOT_INITIALIZED), Timer(nullptr) 
{
	
}


/**
* @brief Init counter
*/
void Counter::Init(HardTimerNumber_t hardTimerNumber)
{
	if (GetStatus() == HARD_TIMER_NOT_INITIALIZED)
	{
		Timer = HardTimerTable[hardTimerNumber];
		if(!IsTimerUsed[hardTimerNumber])
		{
			if(hardTimerNumber == TIMER_2)
			{
				RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
				NVIC_EnableIRQ(TIM2_IRQn);
			}
			else if(hardTimerNumber == TIMER_3)
			{
				RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
				NVIC_EnableIRQ(TIM3_IRQn);
			}
			else if(hardTimerNumber == TIMER_4)
			{
				RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
				NVIC_EnableIRQ(TIM4_IRQn);
			}
			
			Timer->CR1 = 0x01;					// Counter enabled
			Timer->DIER = TIM_DIER_TIE;          // Trigger interrupt enabled
			Timer->DIER |= TIM_DIER_UIE;         // Update interrupt enabled
			SetStatus(HARD_TIMER_INITIALIZED);	
		}
	}
}


/**
* @brief Get value of counter register
* @return value of count register
*/
uint32_t Counter::GetCount()
{
    return Timer->CNT;
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
* @brief Constructor
*/
Pwm::Pwm(): ChannelNumber(NO_CHANNEL)
{
	
}


/**
* @brief Init pwm with max frequency, zero duty cycle and without channel
*/
void Pwm::Init(HardTimerNumber_t hardTimerNumber)
{
	if (GetStatus() == HARD_TIMER_NOT_INITIALIZED)
	{
		Timer = HardTimerTable[hardTimerNumber];
		if(!IsTimerUsed[hardTimerNumber])
		{
			if(hardTimerNumber == TIMER_2)
			{
				RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
			}
			else if(hardTimerNumber == TIMER_3)
			{
				RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
			}
			else if(hardTimerNumber == TIMER_4)
			{
				RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
			}
			SetFrequency(0xFFFF);
			SetDutyCycle(0);
			Timer->EGR = TIM_EGR_UG;
			Timer->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
		}
	}
}


/**
* @brief Set channel
* @param[in] channel
*/
void Pwm::SetChannel(PwmChannel_t channel)
{
	if(channel == CHANNEL_1)
	{
		Timer->CCER = TIM_CCER_CC1E;
		Timer->CCMR1 = (7 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;
	}
	else if(channel == CHANNEL_2)
	{
		Timer->CCER = TIM_CCER_CC2E;
		Timer->CCMR1 = (7 << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE;
	}
	else if(channel == CHANNEL_3)
	{
		Timer->CCER = TIM_CCER_CC3E;
		Timer->CCMR2 = (7 << TIM_CCMR2_OC3M_Pos) | TIM_CCMR2_OC3PE;
	}
	else if(channel == CHANNEL_4)
	{
		Timer->CCER = TIM_CCER_CC4E;
		Timer->CCMR2 = (7 << TIM_CCMR2_OC4M_Pos) | TIM_CCMR2_OC4PE;
	}
	else
	{
		return;
	}
	ChannelNumber = channel;
}


/**
* @brief Set frequency
* @param[in] frequency
*/
void Pwm::SetFrequency(uint32_t f)
{
	Timer->ARR = f;
}


/**
* @brief Set duty cycle
* @param[in] duty cycle in range [0; 100]
*/
void Pwm::SetDutyCycle(uint8_t dutyCycle)
{
	if (dutyCycle <= 100)
	{
		uint32_t value = Timer->ARR / 100 * (100 - dutyCycle);
		switch(ChannelNumber)
		{
			case CHANNEL_1:
			{
				Timer->CCR1 = value;
				break;
			}
			case CHANNEL_2:
			{
				Timer->CCR2 = value;
				break;
			}
			case CHANNEL_3:
			{
				Timer->CCR3 = value;
				break;
			}
			case CHANNEL_4:
			{
				Timer->CCR4 = value;
				break;
			}
			default:
			{
				
			}
		}
	}
}


/**
* @brief Counter interrupt handler
*/
extern "C"
{
	void TIM2_IRQHandler()
	{
		TIM2->SR = 0;
		OverflowsCount++;
	}
	void TIM3_IRQHandler()
	{
		TIM3->SR = 0;
		OverflowsCount++;
	}
	void TIM4_IRQHandler()
	{
		TIM4->SR = 0;
		OverflowsCount++;
	}
}
