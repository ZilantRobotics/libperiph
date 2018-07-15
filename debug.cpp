/**
* @file debug.cpp
* @brief Implementation of debug
*/

#include <uart.hpp>
#include <debug.hpp>
#include "target.hpp"

extern UART UART1;
DebugPort Debug;


void DebugPort::Init()
{
	UART1.Init(UART_1);
}


void DebugPort::Transmit(const uint8_t* arr, const uint8_t& length)
{
	UART1.SendArr(arr, length);
}



void DebugPort::Receive(uint8_t* arr, uint8_t& length)
{
	UART1.GetData(arr, length);
}
