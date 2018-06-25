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
	void SendChar(const char c);
};


#endif //__UART_HPP
