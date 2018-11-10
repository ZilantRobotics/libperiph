/**
* @file target.cpp
* @brief Driver board class implementation
*/

/*
	Short description of file:
	Guaranteed that there will not be more than one instance of this class.
	There are 2 ways: static class and singleton.
	It is assumed that Target class will not have childrens, so static class
	has been selected.
*/

#include "target.hpp"

/**
* @brief Init GPIO type, functions and RCC
*/
void Target::InitGPIO()
{
	///< Some constans
	enum
	{
		GPIO_Mode_IN = 0,										///< Input Mode
		GPIO_Mode_OUT = 1,										///< Output Mode 
		GPIO_Mode_AF = 2,										///< Alternate function Mode 
		GPIO_Mode_AN = 3,										///< Analog Mode 
		
		GPIO_OUTPUT_TYPE_PUSH_PULL = 0,							///< Push pull
		GPIO_OUTPUT_TYPE_OPEN_DRAIN = 1,						///< Open drain
		
		GPIO_PUPDR_NO_PULL_UP_NO_PULL_DOWN = 0,					///< No pull up, no pull down
		GPIO_PUPDR_PULL_UP = 1,									///< Pull up resistor
		GPIO_PUPDR_PULL_DOWN = 2,								///< Pull down resistor
	};
	
    /// Clocking GPIO
    RCC->AHBENR = RCC_AHBENR_GPIOAEN |
				  RCC_AHBENR_GPIOBEN |
				  RCC_AHBENR_GPIOCEN |
				  RCC_AHBENR_GPIODEN |
				  RCC_AHBENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	/// PORTA
	/*
	¹		Moder	PUPDR	OutPutType	Speed	AF			Desription
	PA[0]	AN		-		Push-Pull	Low		-			analog input
	PA[1]	AN		-		Push-Pull	Low		-			analog input
	PA[2]	AF		pu		Push-Pull	Low		USART2_TX			
	PA[3]	AF		npu npd	Push-Pull	Low		USART2_RX			
	*/
	GPIOA->MODER |= (GPIO_Mode_AN << GPIO_MODER_MODER0_Pos) |
					(GPIO_Mode_AN << GPIO_MODER_MODER1_Pos) |
					(GPIO_Mode_AF << GPIO_MODER_MODER2_Pos) |
					(GPIO_Mode_AF << GPIO_MODER_MODER3_Pos) ;
	GPIOA->OTYPER = 0;
	GPIOA->OSPEEDR = 0;
	GPIOA->PUPDR = (GPIO_PUPDR_PULL_UP << GPIO_PUPDR_PUPDR2_Pos) |
				   (GPIO_PUPDR_NO_PULL_UP_NO_PULL_DOWN << GPIO_PUPDR_PUPDR3_Pos);
	GPIOA->AFR[0] = (7 << GPIO_AFRL_AFRL2_Pos) |
					(7 << GPIO_AFRL_AFRL3_Pos);
	
	/// PORTB
	/*
	¹		Moder	PUPDR	OutPutType	Speed	AF			Desription
	PB[6]	AF		-		-			-		TIM4_ch_1	Motor 2 pin 1
	PB[7]	AF		-		-			-		TIM4_ch_2	Motor 2 pin 2
	*/
	GPIOB->MODER =	(GPIO_Mode_AF << GPIO_MODER_MODER6_Pos)  |
					(GPIO_Mode_AF << GPIO_MODER_MODER7_Pos);
	GPIOB->OTYPER = 0;
	GPIOB->OSPEEDR = 0;
	
	/// PORTC
	/*
	¹		Moder	PUPDR	OutPutType	Speed	AF			Desription
	PC[4]	OUT		-		-			-		UART1, TX	
	PC[5]	IN		pd		Push-Pull	-		UART1, RX	
	PC[6]	AF		-		-			-		TIM3_ch_1	Motor 1 pin 1
	PC[7]	AF		-		-			-		TIM3_ch_2	Motor 1 pin 2
	*/
    GPIOC->MODER =	(GPIO_Mode_AF << GPIO_MODER_MODER4_Pos) |
					(GPIO_Mode_AF << GPIO_MODER_MODER5_Pos) |
					(GPIO_Mode_AF << GPIO_MODER_MODER6_Pos) |
					(GPIO_Mode_AF << GPIO_MODER_MODER7_Pos);					
	GPIOC->OTYPER = 0;
	GPIOC->OSPEEDR = 0;
	GPIOC->PUPDR = (GPIO_PUPDR_PULL_UP << GPIO_PUPDR_PUPDR4_Pos) |
				   (GPIO_PUPDR_NO_PULL_UP_NO_PULL_DOWN << GPIO_PUPDR_PUPDR5_Pos);
	GPIOC->AFR[0] = (7 << GPIO_AFRL_AFRL4_Pos) |
					(7 << GPIO_AFRL_AFRL5_Pos) |
					(2 << GPIO_AFRL_AFRL6_Pos) |
					(2 << GPIO_AFRL_AFRL7_Pos);
	
	/// PORTD
	/*
	¹		Moder	PUPDR	OutPutType	Speed	AF			Desription
	PD[10]	IN		pd		Push-Pull	-		-			Left Encoder pin1 - external interrupt
	PD[11]	IN		pd		Push-Pull	-		-			Left Encoder pin2 - input
	PD[12]	IN		pd		Push-Pull	-		-			Right Encoder pin1 - external interrupt
	PD[13]	IN		pd		Push-Pull	-		-			Right Encoder pin2 - input
	*/
    GPIOD->MODER =	(GPIO_Mode_IN << GPIO_MODER_MODER10_Pos) |
					(GPIO_Mode_IN << GPIO_MODER_MODER11_Pos) |
					(GPIO_Mode_IN << GPIO_MODER_MODER12_Pos) |
					(GPIO_Mode_IN << GPIO_MODER_MODER13_Pos);
	GPIOD->PUPDR =	(GPIO_PUPDR_PULL_DOWN << GPIO_PUPDR_PUPDR10_Pos) |
					(GPIO_PUPDR_PULL_DOWN << GPIO_PUPDR_PUPDR11_Pos) |
					(GPIO_PUPDR_PULL_DOWN << GPIO_PUPDR_PUPDR12_Pos) |
					(GPIO_PUPDR_PULL_DOWN << GPIO_PUPDR_PUPDR13_Pos);
	
	/// PORTE
	/*
	¹		Moder	PUPDR	OutPutType	Speed	AF			Desription
	PE[8]	OUT		-		Push-Pull	Low		-			LD4 (leds rotation)
	PE[9]	OUT		-		Push-Pull	Low		- 			LD3 (leds rotation)
	PE[10]	OUT		-		Push-Pull	Low		-			LD5 (leds rotation)
	PE[11]	OUT		-		Push-Pull	Low		-			LD7 (leds rotation)
	PE[12]	OUT		-		Push-Pull	Low		-			LD9 (leds rotation)
	PE[13]	OUT		-		Push-Pull	Low		-			LD10 (leds rotation)
	PE[14]	OUT		-		Push-Pull	Low		-			LD8 (leds rotation)
	PE[15]	OUT		-		Push-Pull	Low		-			LD6 (leds rotation)
	*/
	GPIOE->MODER =	(GPIO_Mode_OUT << GPIO_MODER_MODER8_Pos)  |
					(GPIO_Mode_OUT << GPIO_MODER_MODER9_Pos)  |
					(GPIO_Mode_OUT << GPIO_MODER_MODER10_Pos) |
					(GPIO_Mode_OUT << GPIO_MODER_MODER11_Pos) |
					(GPIO_Mode_OUT << GPIO_MODER_MODER12_Pos) |
					(GPIO_Mode_OUT << GPIO_MODER_MODER13_Pos) |
					(GPIO_Mode_OUT << GPIO_MODER_MODER14_Pos) |
					(GPIO_Mode_OUT << GPIO_MODER_MODER15_Pos);
	GPIOE->OTYPER = 0;
	GPIOE->OSPEEDR = 0;
}


/**
* @brief Turn on this led from board
* @param ledNumber - number from LD_MIN to LD_MAX
*/
void Target::LedOn(uint8_t ledNumber)
{
	if ( (ledNumber <= LD_MAX) && (ledNumber >= LD_MIN) )
		GPIOE->ODR |= (1 << ledNumber);
}


/**
* @brief Turn off this led from board
* @param ledNumber - number from LD_MIN to LD_MAX
*/
void Target::LedOff(uint8_t ledNumber)
{
	if ( (ledNumber <= LD_MAX) && (ledNumber >= LD_MIN) )
		GPIOE->ODR &= ~(1 << ledNumber);
}
