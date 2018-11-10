/**
* @file uart.cpp
* @brief uart implementation
*/

#include "uart.hpp"
#include "target.hpp"
#include <string.h>

UART UART1;
UART UART2;

enum
{
	BAUD_RATE_600 = SYSCLOCK/600,		///< 13333
	BAUD_RATE_1200 = SYSCLOCK/1200,		///< 6666
	BAUD_RATE_2400 = SYSCLOCK/2400,		///< 3333
	BAUD_RATE_4800 = SYSCLOCK/4800,		///< 1666
	BAUD_RATE_9600 = SYSCLOCK/9600,		///< 833
	
	BAUD_RATE_14400 = SYSCLOCK/14400,	///< 555
	BAUD_RATE_19200 = SYSCLOCK/19200,	///< 416
	BAUD_RATE_28800 = SYSCLOCK/28800,	///< 277
	BAUD_RATE_38400 = SYSCLOCK/38400,	///< 208
	BAUD_RATE_56000 = SYSCLOCK/56000,	///< 142
	
	BAUD_RATE_57600 = SYSCLOCK/57600,	///< 138
	BAUD_RATE_115200 = SYSCLOCK/115200, ///< 69
	BAUD_RATE_128000 = SYSCLOCK/128000,	///< 62
	BAUD_RATE_256000 = SYSCLOCK/260000,	///< 30

};


/**
* @brief Init UART
*/
void UART::Init(UartNumber_t uartNumber)
{
	// Check correctness
	if (IsItInit)
		return;
	
	// Select UART register and clock and interrupt enable
	switch (uartNumber)
	{
		case UART_1:
			UARTX = USART1;
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
			NVIC_EnableIRQ(USART1_IRQn);
			break;
		case UART_2:
			UARTX = USART2;
			RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
			NVIC_EnableIRQ(USART2_IRQn);
			break;
		case UART_3:
			UARTX = USART3;
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
			NVIC_EnableIRQ(USART3_IRQn);
			break;
		case UART_4:
			UARTX = UART4;
			RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
			NVIC_EnableIRQ(UART4_IRQn);
			break;
		case UART_5:
			UARTX = UART5;
			RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
			NVIC_EnableIRQ(UART5_IRQn);
			break;
		default:
			return;
	}
	
	// 1. Configure UART
	UARTX->CR3 = USART_CR3_DMAT |		// DMA mode is disabled for transmission
					 USART_CR3_DMAR; 	// DMA mode is disabled for reception	
	UARTX->CR1 = USART_CR1_RXNEIE |		// RX Interrupt Enable	
				 USART_CR1_TE |			// Transmitter enable
				 USART_CR1_RE;			// Receiver enable;
	
	// 2. Select the desired baud rate using the baud rate register USART_BRR 
	UARTX->BRR = BAUD_RATE_115200;
	
	// 3. Enable USART
	UARTX->CR1 |=  USART_CR1_UE;
	IsItInit = true;
}


/**
* @brief Transmit null-terminated string
* @param str - pointer to null-terminated string
*/
void UART::TransmitString(const uint8_t* str)
{
	if(!IsItInit)
		return;
	
	while(*str != 0x00)
		TransmitChar( *str++ );
}


/**
* @brief Send array of data
* @param arr - pointer to array
* @param len - length of array
*/
void UART::TransmitArr(const uint8_t* arr, uint8_t length)
{
	if(!IsItInit)
		return;

	while(length--)
		TransmitChar( *arr++ );
}


/**
* @brief Send one data byte
* @param byte - data byte
*/
void UART::TransmitChar(const uint8_t byte) 
{
	UARTX->TDR = byte;
	while( !(UARTX->ISR & USART_ISR_TC) );
}


/**
* @brief Get pointer on data array and length of array
* @param ptrArr - pointer on data array
* @param length - link on length of array
*/
void UART::ReceiveArr(uint8_t* ptrArr, uint8_t& length)
{
	BufferRX.PopAll((uint8_t*)ptrArr, length);
}


/**
* @brief Interrupt handler UART
*/
extern "C"
{
	void USART1_IRQHandler()
	{
		// If Read data register not empty
		if ( (USART1->ISR & USART_ISR_RXNE) )
		{	
			UART1.BufferRX.Push(USART1->RDR);
		}
		// If Overrun error
		if ( USART1->ISR & USART_ISR_ORE)
		{
			USART1->ICR |= USART_ICR_ORECF;
		}
	}
	void USART2_IRQHandler()
	{
		// If Read data register not empty
		if ( (USART2->ISR & USART_ISR_RXNE) )
		{	
			UART2.BufferRX.Push(USART2->RDR);
		}
		// If Overrun error
		if ( USART2->ISR & USART_ISR_ORE)
		{
			USART2->ICR |= USART_ICR_ORECF;
		}
	}
}


/**
* @brief Push byte to buffer
* @param byte - byte of data
*/
void Buffer::Push(const uint8_t byte)
{
	if (Size != MaxSize)
	{
		Arr[Size++] = byte;
	}
}


/**
* @brief Get pointer of buffer head and his length
* @param destptr - pointer of buffer head, which data will be written
* @param length - number of data bytes, which will be written
*/
void Buffer::PopAll(uint8_t* destptr, uint8_t& length)
{
	memcpy(destptr, Arr, Size);
	length = Size;
	Size = 0;
}

