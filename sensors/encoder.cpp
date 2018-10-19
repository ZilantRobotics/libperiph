/**
* @file encoder.cpp
* @brief Implementation of encoder sensor
*/

#include "encoder.hpp"

static uint32_t Counter = 0;

void Encoder::Init()
{
	
	//EXTI->FTSR |= EXTI_FTSR_TR10;					/// по спадающему фронту
	EXTI->RTSR |= EXTI_RTSR_TR10;					/// по нарастающему фронту
	EXTI->IMR |= EXTI_IMR_MR10;						/// разрешаем прерывание выводов
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PD; 	/// качестве источника прерываний PD[10]
	NVIC_EnableIRQ(EXTI15_10_IRQn);					/// –азрешить прерывание EXTI10 в NVIC
}


uint32_t Encoder::GetPulses()
{
	return 777;
}


extern "C"
{
	void EXTI15_10_IRQHandler(void)
	{
		Counter++;
		EXTI->PR |= EXTI_PR_PR10;
	}
}
