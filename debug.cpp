/**
* @file debug.cpp
* @brief Implementation of Debug Port
*/

#include <uart.hpp>
#include <debug.hpp>
#include "target.hpp"

extern UART UART1;
DebugPort Debug;


/**
* @brief Init debug port
*/
void DebugPort::Init()
{
	UART1.Init(UART_1);
}


/**
* @brief Transmit array of data to Debug Port
* @param arr - pointer to array
* @param length - length of array
*/
template <class T>
void DebugPort::Transmit(const T* arr, const uint8_t& length)
{
	UART1.TransmitArr(arr, length);
}
template void DebugPort::Transmit(const uint8_t* arr, const uint8_t& length);
template void DebugPort::Transmit(const char* arr, const uint8_t& length);


/**
* @brief Transmit null-terminated string to Debug Port
* @param str - pointer to null-terminated string
*/
template <class T>
void DebugPort::Transmit(const T* str)
{
	UART1.TransmitString(str);
}
template void DebugPort::Transmit(const uint8_t* arr);
template void DebugPort::Transmit(const char* arr);


/**
* @brief Receive array of data from Debug Port
* @param arr - pointer to array
* @param length - length of array
*/
template <class T>
void DebugPort::Receive(T* arr, uint8_t& length)
{
	UART1.ReceiveArr(arr, length);
}
template void DebugPort::Receive(uint8_t* arr, uint8_t& length);
template void DebugPort::Receive(char* arr, uint8_t& length);
