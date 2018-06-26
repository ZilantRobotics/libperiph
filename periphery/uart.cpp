/**
* @file uart.cpp
* @brief Реализация UART
*/

#include <uart.hpp>
//uint8_t usartData;


/**
* @brief Инициализация UART
*/
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


/**
* @brief Отправить массив данных
*/
void UART::SendArr(const uint8_t* arr, uint8_t length)
{
	while(length--)
	{
		SendChar( *(arr++) );
	}
}


/**
* @brief Отправить один байт данных
* @param byte - байт
*/
void UART::SendChar(const uint8_t byte) 
{
	USART1->TDR = byte;
	while( !(USART1->ISR & USART_ISR_TC) );
}


/*
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
*/
