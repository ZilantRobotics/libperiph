#ifndef __UART_HPP
#define __UART_HPP

#include "stm32f3xx.h"
#include "target.hpp"


/**
* @brief Äנאיגונ UART
*/
struct UART
{
	void Init();
	void SendArr(const uint8_t* arr, uint8_t length);
	void SendChar(const uint8_t byte);
};


#endif //__UART_HPP
