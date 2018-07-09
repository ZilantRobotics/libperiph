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
* @brief Буффер данных приема
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
* @brief Драйвер UART
*/
class UART
{
	public:
		UART(): IsItInit(UART_IS_NOT_INITIALIZED) {};
		void Init(uint8_t uartNumber);
		void SendArr(const uint8_t* arr, const uint8_t& len);
		void GetData(uint8_t* ptrArr, uint8_t& length);
		Buffer BufferRX;
	private:
		void SendChar(const uint8_t byte);
		uint8_t IsItInit;
		uint8_t UartNumber;
		USART_TypeDef* UARTX;
};

#endif //__UART_HPP
