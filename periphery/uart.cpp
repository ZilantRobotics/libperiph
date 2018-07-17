/**
* @file uart.cpp
* @brief uart implementation
*/

#include <uart.hpp>
#include <string.h>	// for memcpy (why not <cstring>???)
#include "target.hpp"

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
void UART::Init(uint8_t uartNumber)
{
	// Check correctness
	if (IsItInit)
		return;
	
	// Select UART register and clock and interrupt enable
	switch (uartNumber)
	{
		case 1:
			UARTX = USART1;
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
			NVIC_EnableIRQ(USART1_IRQn);
			break;
		case 2:
			UARTX = USART2;
			RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
			NVIC_EnableIRQ(USART2_IRQn);
			break;
		case 3:
			UARTX = USART3;
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
			NVIC_EnableIRQ(USART3_IRQn);
			break;
		case 4:
			UARTX = UART4;
			RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
			NVIC_EnableIRQ(UART4_IRQn);
			break;
		case 5:
			UARTX = UART5;
			RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
			NVIC_EnableIRQ(UART5_IRQn);
			break;
		default:
			return;
	}
	
	// UART configuration
	enum
	{
		CR1_CONFIG = 						// Word length = 8 bit
											// Oversampling by 16
											// Parity control disabled
											// Parity selection (if control enabled)
					0*USART_CR1_TXEIE | 	// TX Interrupt Enable
					1*USART_CR1_RXNEIE |	// RX Interrupt Enable	
											// 1 stop bit
											// CTS disable
											// RTS disable
					1*USART_CR1_TE |		// Transmitter enable
					1*USART_CR1_RE,			// Receiver enable
		CR2_CONFIG =0,						// Default
		CR3_CONFIG =USART_CR3_DMAT |		// DMA mode is disabled for transmission
					USART_CR3_DMAR, 		// DMA mode is disabled for reception
	};
	
	// 1. Configure UART
	UARTX->CR3 = CR3_CONFIG;	
	UARTX->CR1 = CR1_CONFIG;
	
	// 2. Select the desired baud rate using the baud rate register USART_BRR 
	UARTX->BRR = BAUD_RATE_115200;
	
	// 3. Enable USART
	UARTX->CR1 |=  USART_CR1_UE;
	IsItInit = UART_IS_INITIALIZED;
}


/**
* @brief Transmit null-terminated string
* @param str - pointer to null-terminated string
*/
template <class T>
void UART::TransmitString(const T* str)
{
	// Check correctness
	if(!IsItInit)
		return;
	
	// Main algorithm
	while(*str != 0x00)
		TransmitChar( *str++ );
}
template void UART::TransmitString(const uint8_t* str);
template void UART::TransmitString(const char* str);


/**
* @brief Send array of data
* @param arr - pointer to array
* @param len - length of array
*/
template <class T>
void UART::TransmitArr(const T* arr, uint8_t length)
{
	// Check correctness
	if(!IsItInit)
		return;
	
	// Main algorithm
	while(length--)
		TransmitChar( *arr++ );
}
template void UART::TransmitArr(const uint8_t* arr, uint8_t length);
template void UART::TransmitArr(const char* arr, uint8_t length);


/**
* @brief Send one data byte
* @param byte - data byte
*/
template <class T>
void UART::TransmitChar(const T byte) 
{
	UARTX->TDR = byte;
	while( !(UARTX->ISR & USART_ISR_TC) );
}
template void UART::TransmitChar(const uint8_t byte);
template void UART::TransmitChar(const char byte);


/**
* @brief Get pointer on data array and length of array
* @param ptrArr - pointer on data array
* @param length - link on length of array
*/
template <class T>
void UART::ReceiveArr(T* ptrArr, uint8_t& length)
{
	BufferRX.PopAll((uint8_t*)ptrArr, length);
}
template void UART::ReceiveArr(uint8_t* ptrArr, uint8_t& length);
template void UART::ReceiveArr(char* ptrArr, uint8_t& length);


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

