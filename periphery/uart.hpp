#ifndef __UART_HPP
#define __UART_HPP

#include "stm32f3xx.h"
#include "target.hpp"


/**
* @brief Драйвер UART
*/
struct UART
{
	void Init();
	void SendArr(const uint8_t* arr, const uint8_t& len);
	void SendChar(const uint8_t byte);
	void GetData(uint8_t* ptrArr, uint8_t& length);
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

#endif //__UART_HPP
