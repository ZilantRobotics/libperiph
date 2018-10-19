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

#include <target.hpp>

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
    RCC->AHBENR = RCC_AHBENR_GPIOAEN |							// Clocking GPIO A
				  RCC_AHBENR_GPIOBEN |							// Clocking GPIO B
				  RCC_AHBENR_GPIOCEN |							// Clocking GPIO C
				  RCC_AHBENR_GPIODEN |							// Clocking GPIO D
				  RCC_AHBENR_GPIOEEN;							// Clocking GPIO E
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;						// Clocking SYSCFG
	
	/// PORTA
	GPIOA->MODER |= (GPIO_Mode_AN << GPIO_MODER_MODER0_Pos) |	// PORTA0, analog input
					(GPIO_Mode_AN << GPIO_MODER_MODER1_Pos) |	// PORTA1, analog input
					(GPIO_Mode_AF << GPIO_MODER_MODER2_Pos) |	// PORTA2, USART2_TX
					(GPIO_Mode_AF << GPIO_MODER_MODER3_Pos) ;	// PORTA3, USART2_RX
	GPIOA->OTYPER = 0;											// Output type, Push-Pull
	GPIOA->OSPEEDR = 0;											// Speed - Low
	GPIOA->PUPDR = (GPIO_PUPDR_PULL_UP << 2*2) | 				// PORTA2, UART2, TX
				   (GPIO_PUPDR_NO_PULL_UP_NO_PULL_DOWN << 2*3); // PORTA3, UART2, RX
	GPIOA->AFR[0] = (7 << GPIO_AFRL_AFRL2_Pos) | 				// PORTA2, UART2, TX
					(7 << GPIO_AFRL_AFRL3_Pos);					// PORTA3, UART2, RX
	
	/// PORTB
    GPIOB->MODER =	(GPIO_Mode_OUT << 0*2)  |					// PORTB0,
					(GPIO_Mode_OUT << 1*2)  |					// PORTB1,
					(GPIO_Mode_OUT << 2*2)  |					// PORTB2,
					(GPIO_Mode_OUT << 3*2)  |					// PORTB3,
					(GPIO_Mode_OUT << 4*2)  |					// PORTB4,
					(GPIO_Mode_OUT << 5*2)  |					// PORTB5,
					(GPIO_Mode_OUT << 6*2)  |					// PORTB6,
					(GPIO_Mode_OUT << 7*2)  |					// PORTB7,
					(GPIO_Mode_OUT << 8*2)  |					// PORTB8,
					(GPIO_Mode_OUT << 9*2)  |					// PORTB9,
					(GPIO_Mode_OUT << 10*2) |					// PORTB10,
					(GPIO_Mode_OUT << 11*2);					// PORTB11,
	GPIOB->OTYPER = GPIO_OUTPUT_TYPE_PUSH_PULL;					// Output type, Push-Pull
	GPIOB->OSPEEDR = 0;											// Speed - Low
	
	/// PORTC
    GPIOC->MODER =	(GPIO_Mode_AF << GPIO_MODER_MODER4_Pos)  |	// PORTC4, UART1, TX
					(GPIO_Mode_AF << GPIO_MODER_MODER5_Pos);	// PORTC5, UART1, RX
	GPIOC->OTYPER = 0;											// Output push-pull 
	GPIOC->OSPEEDR = 0;											// speed
	GPIOC->PUPDR = (GPIO_PUPDR_PULL_UP << 2*4) | 				// PORTC4, UART1, TX
				   (GPIO_PUPDR_NO_PULL_UP_NO_PULL_DOWN << 2*5); // PORTC5, UART1, RX
	GPIOC->AFR[0] = (7 << GPIO_AFRL_AFRL4_Pos) | 				// PORTC4, UART1, TX
					(7 << GPIO_AFRL_AFRL5_Pos);					// PORTC5, UART1, RX
	
	/// PORTD
    GPIOD->MODER =	(GPIO_Mode_IN << 10*2);						// PORTD10 - external interrupt
	GPIOD->PUPDR = (GPIO_PUPDR_PULL_DOWN << 10*2);				// 
	
	/// PORTE
	GPIOE->MODER =	(GPIO_Mode_OUT << 8*2)  |					// PORTE8,  LD4,
					(GPIO_Mode_OUT << 9*2)	|					// PORTE9,  LD3,
					(GPIO_Mode_OUT << 10*2) |					// PORTE10, LD5,
					(GPIO_Mode_OUT << 11*2) |					// PORTE11, LD7,
					(GPIO_Mode_OUT << 12*2) |					// PORTE12, LD9,
					(GPIO_Mode_OUT << 13*2) |					// PORTE13, LD10,
					(GPIO_Mode_OUT << 14*2) |					// PORTE14, LD8,
					(GPIO_Mode_OUT << 15*2);					// PORTE15, LD6,
	GPIOE->OTYPER = 0;											// Output type, Push-Pull
	GPIOE->OSPEEDR = 0;											// Speed - Low
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
