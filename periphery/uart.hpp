#ifndef __UART_HPP
#define __UART_HPP

#include "stm32f3xx.h"


enum
{
	// IsItInit
	UART_IS_NOT_INITIALIZED = 0,
	UART_IS_INITIALIZED = 1,
	
	// UartNumber
	UART_1 = 1,
	UART_2 = 2,
	UART_3 = 3,
	UART_4 = 4,
	UART_5 = 5,
};

/**
* @brief Buffer of receive data
*/
class Buffer
{
	public:
		enum
		{
			BUFFER_SIZE = 255,
		};
		Buffer(): MaxSize(BUFFER_SIZE), Size(0) {};
		void Push(const uint8_t byte);
		void PopAll(uint8_t* ptrArr, uint8_t& length);
	
	private:
		const uint8_t MaxSize;
		uint8_t Size;
		uint8_t Arr[BUFFER_SIZE];
};


/**
* @brief Driver UART
*/
class UART
{
	public:
		UART(): IsItInit(UART_IS_NOT_INITIALIZED) {};
		void Init(uint8_t uartNumber);
		template <class T>
		void TransmitString(const T* str);
		template <class T>
		void TransmitArr(const T* arr, uint8_t length);
		template <class T>
		void ReceiveArr(T* ptrArr, uint8_t& length);
		Buffer BufferRX;
	private:
		template <class T>
		void TransmitChar(const T byte);
		uint8_t IsItInit;
		uint8_t UartNumber;
		USART_TypeDef* UARTX;
};

#endif //__UART_HPP
