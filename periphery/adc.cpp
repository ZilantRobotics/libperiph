/**
* @file adc.cpp
* @brief Implementation of ADC
*/

#include <adc.hpp>
ADC Adc;

/**
* @brief Init ADC
* @note Software procedure to enable the ADC
*/
void ADC::Init()	
{
	// ADC1 Clocking and Interrupt enable
	RCC->AHBENR |= RCC_AHBENR_ADC12EN;
	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV1;	// prescaler divider
	
	// Configuration
	ADC1_2_COMMON->CCR = 1 << ADC12_CCR_CKMODE_Pos;
	ADC1->IER |= ADC_IER_ADRDYIE;
	ADC1->CFGR |= ADC_CFGR_DISCEN;
	ADC1->IER |= ADC_IER_EOCIE;
	ADC1->CFGR |= ADC_CFGR_DISCEN;
	ADC1->SQR1 |=  (0x0001 << ADC_SQR1_SQ4_Pos);
	ADC1->SQR1 |=  (0x0001 << ADC_SQR1_SQ3_Pos);
	ADC1->SQR1 |=  (0x0001 << ADC_SQR1_SQ2_Pos);
	ADC1->SQR1 |=  (0x0001 << ADC_SQR1_SQ1_Pos);
	ADC1->SQR4 |=  (0x0001 << ADC_SQR4_SQ16_Pos);
	
	// Enable the ADC and wait until ADC is ready to start conversion
	ADC1->CR = ADC_CR_ADEN;				
	while( !(ADC1->ISR & ADC_ISR_ADRDY) );
}


/**
* @brief Convert the ADC and return the value
* @return value of ADC
*/
uint16_t ADC::Do()
{
	ADC1->CR |= ADC_CR_ADSTART;
	while( !(ADC1->ISR & ADC_ISR_EOC) );
	return ADC1->DR;
}
