/**
* @file target.cpp
* @brief Реализация базового класса драйвера платы
*/

#include <target.hpp>

TargetBase Target;

/**
* @brief Инициализация GPIO
*/
void TargetBase::InitGPIO()
{
    // Clocking GPIO
    RCC->AHBENR = RCC_AHBENR_GPIOAEN |		// Clocking GPIO A
				  RCC_AHBENR_GPIOBEN |		// Clocking GPIO B
				  RCC_AHBENR_GPIODEN |		// Clocking GPIO D
				  RCC_AHBENR_GPIOEEN;		// Clocking GPIO E
	
	// PORTA
	GPIOA->MODER |= (GPIO_Mode_AN << GPIO_MODER_MODER0_Pos) |	// PORTA0, analog input
					(GPIO_Mode_AN << GPIO_MODER_MODER1_Pos);	// PORTA1, analog input
	
	GPIOA->OTYPER = 0;	// Output type, Push-Pull
	GPIOA->OSPEEDR = 0;	// Speed - Low
	
	// PORTB
    GPIOB->MODER =	(GPIO_Mode_OUT << 0*2)  |		// PORTB0,
					(GPIO_Mode_OUT << 1*2)  |		// PORTB1,
					(GPIO_Mode_OUT << 2*2)  |		// PORTB2,
					(GPIO_Mode_OUT << 3*2)  |		// PORTB3,
					(GPIO_Mode_OUT << 4*2)  |		// PORTB4,
					(GPIO_Mode_OUT << 5*2)  |		// PORTB5,
					(GPIO_Mode_OUT << 6*2)  |		// PORTB6,
					(GPIO_Mode_OUT << 7*2)  |		// PORTB7,
					(GPIO_Mode_OUT << 8*2)  |		// PORTB8,
					(GPIO_Mode_OUT << 9*2)  |		// PORTB9,
					(GPIO_Mode_OUT << 10*2) |		// PORTB10,
					(GPIO_Mode_OUT << 11*2);		// PORTB11,
    
	GPIOB->OTYPER = 0;	// Output type, Push-Pull
	GPIOB->OSPEEDR = 0;	// Speed - Low
	
	// PORTD
    GPIOD->MODER =	(GPIO_Mode_OUT << 0*2)  |		// PORTB0,
					(GPIO_Mode_OUT << 2*2)  |		// PORTB2,
					(GPIO_Mode_OUT << 4*2)  |		// PORTB4,
					(GPIO_Mode_OUT << 6*2)  |		// PORTB6,
					(GPIO_Mode_OUT << 8*2)  |		// PORTB8,
					(GPIO_Mode_OUT << 10*2) |		// PORTB10,
					(GPIO_Mode_OUT << 12*2) |		// PORTB12,
					(GPIO_Mode_OUT << 14*2);		// PORTB14,
	
	GPIOD->OTYPER = 0;	// Output type, Push-Pull
	GPIOD->OSPEEDR = 0;	// Speed - Low
	
	// PORTE
	GPIOE->MODER =	(GPIO_Mode_OUT << 8*2)  |		// PORTE8,  LD4,
					(GPIO_Mode_OUT << 9*2)	|		// PORTE9,  LD3,
					(GPIO_Mode_OUT << 10*2) |		// PORTE10, LD5,
					(GPIO_Mode_OUT << 11*2) |		// PORTE11, LD7,
					(GPIO_Mode_OUT << 12*2) |		// PORTE12, LD9,
					(GPIO_Mode_OUT << 13*2) |		// PORTE13, LD10,
					(GPIO_Mode_OUT << 14*2) |		// PORTE14, LD8,
					(GPIO_Mode_OUT << 15*2);		// PORTE15, LD6,
	
	GPIOE->OTYPER = 0;	// Output type, Push-Pull
	GPIOE->OSPEEDR = 0;	// Speed - Low
}


/**
* @brief Включить указанный светодиод на плате
*/
void TargetBase::LedOn(uint8_t ledNumber)
{
	GPIOE->ODR |= (1 << ledNumber);
}


/**
* @brief Выключить указанный светодиод на плате
*/
void TargetBase::LedOff(uint8_t ledNumber)
{
	GPIOE->ODR &= ~(1 << ledNumber);
}

