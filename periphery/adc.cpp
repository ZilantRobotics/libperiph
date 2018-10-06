/**
* @file adc.cpp
* @brief Implementation of ADC
*/

#include <adc.hpp>
ADC Adc;

/// ADC1_IN1  - A0
/// ADC1_IN16 - temperature sensor

/**
* @brief Init ADC
* @note Software procedure to enable the ADC
*/
void ADC::Init()	
{
	enum
	{
		CHANNEL_1  = 1,			///< A0
		CHANNEL_16 = 16,		///< Temperature sensor
	};
	/// ADC1 Clocking enable
	RCC->AHBENR |= RCC_AHBENR_ADC12EN;				/// Clocking enable
	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV1;			/// Prescaler divider
	
	/// ADC1 Configuration
	ADC1_2_COMMON->CCR = 1 << ADC12_CCR_CKMODE_Pos;	/// Synchronous clock mode
	ADC1->IER |= ADC_IER_ADRDYIE;					/// ADRDY (ADC ready) interrupt enable
	ADC1->CFGR |= ADC_CFGR_DISCEN;					/// Discontinuous mode for regular channels enabled
	ADC1->IER |= ADC_IER_EOCIE;						/// EOC (End of regular conversion) interrupt enable
	ADC1->SQR1 |= (CHANNEL_16 << ADC_SQR1_SQ1_Pos);	/// 1st conversion in regular sequence
	ADC1->SMPR2 = (7 << ADC_SMPR2_SMP16_Pos);
	
	/// Enable the ADC1 and wait until ADC is ready to start conversion
	ADC1->CR = ADC_CR_ADEN;							/// Enable the ADC1
	ADC1_2_COMMON->CCR |= ADC12_CCR_TSEN;
	while( !(ADC1->ISR & ADC_ISR_ADRDY) );			/// Wait until ADC is ready to start conversion
}
//{
//	enum
//	{
//		CHANNEL_1  = 1,
//		CHANNEL_16 = 16,
//	};
//	/// ADC1 Clocking enable
//	RCC->AHBENR |= RCC_AHBENR_ADC12EN;				/// Clocking enable
//	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV1;			/// Prescaler divider
//	
//	/// ADC1 Configuration
//	ADC1_2_COMMON->CCR = 1 << ADC12_CCR_CKMODE_Pos;
//	ADC1->IER |= ADC_IER_ADRDYIE;					/// ADRDY (ADC ready) interrupt enable
//	ADC1->CFGR |= ADC_CFGR_DISCEN;					/// Discontinuous mode for regular channels enabled
//	ADC1->IER |= ADC_IER_EOCIE;						/// EOC (End of regular conversion) interrupt enable
//	ADC1->SQR1 |=  (CHANNEL_1 << ADC_SQR1_SQ1_Pos);	/// 1st conversion in regular sequence
//	
//	/// Enable the ADC1 and wait until ADC is ready to start conversion
//	ADC1->CR = ADC_CR_ADEN;							/// Enable the ADC1
//	while( !(ADC1->ISR & ADC_ISR_ADRDY) );			/// Wait until ADC is ready to start conversion
//}


/**
* @brief Convert the ADC and return the value
* @return value of ADC
*/
uint16_t ADC::Do()
{
	ADC1->CR |= ADC_CR_ADSTART;
	while( !(ADC1->ISR & ADC_ISR_EOC) );
	//if( !(ADC1->ISR & ADC_ISR_EOC) )
	//	ADC1->CR |= ADC_CR_ADSTART;
	return ADC1->DR;
}
