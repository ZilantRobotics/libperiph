/**
* @file uart.cpp
* @brief Реализация UART
*/

#include <uart.hpp>
uint8_t usartData;

void UART::Init()
{
	//__enable_irq();
	// Clock and interrupt enable
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//NVIC_EnableIRQ(USART1_IRQn);
	
	//USART
     //USART1->CR1 = 0;//Reset
     //USART1->CR2 = 0;//Reset
     //USART1->CR3 = 0;//Reset
	USART1->BRR = 833;//set Baudrate to 9600
	USART1->CR1 |= 	//					// Word length = 8 bit
					//					// Oversampling by 16
					//USART_CR1_RE |	// Receiver enable
										// Parity control disabled
					USART_CR1_TXEIE | 	// TX Interrupt Enable
					USART_CR1_RXNEIE; 	// RX Interrupt Enable	
										// 1 stop bit
										// CTS disable
										// RTS disable
	USART1->CR1 |=  USART_CR1_UE;  
	USART1->CR1 |= USART_CR1_TE;		// Transmitter enable
	USART1->ICR = 1;	// clear
	
}

void UART::SendChar(const char c) 
{
	USART1->TDR = (c & 0xFF);
	volatile uint32_t val_1 = USART1->ISR;
	while( !(USART1->ISR & USART_ISR_TC) )//Data transfered to shift register
	{
		volatile uint32_t val_2 = USART1->ISR;
	}
}


extern "C"
{
	void USART1_IRQHandler()
	{
		//Проверяем, действительно ли прерывание вызвано приемом нового байта
		if ( !(USART1->ISR & USART_ISR_RXNE) )
		{	
			usartData = USART1->TDR;	
		}
	}
}
