/**
* @file debug.cpp
* @brief Implementation of Debug Port
*/

#include "uart.hpp"
#include "debug.hpp"
#include "target.hpp"
#include "text.hpp"

extern UART UART1;
DebugPort Debug;


/**
* @brief Init debug port
*/
void DebugPort::Init()
{
	UART1.Init(UART::UART_1);
}


/**
* @brief Transmit array of data to Debug Port
* @param arr - pointer to array
* @param length - length of array
*/
void DebugPort::Transmit(const uint8_t* arr, const uint8_t& length)
{
	UART1.TransmitArr(arr, length);
}


/**
* @brief Transmit null-terminated string to Debug Port
* @param str - pointer to null-terminated string
*/
void DebugPort::Transmit(const uint8_t* str)
{
	UART1.TransmitString(str);
}


/**
* @brief Transmit Value to Debug Port
* @param value - value being transmitting
*/
void DebugPort::Transmit(uint32_t value)
{
	uint8_t buffer[5];	// sizeof(uint32_t) + sizeof('\0')
	num2str(value, buffer);
	Debug.Transmit(buffer);
	Debug.Transmit((uint8_t*)"\n");
}


/**
* @brief Receive array of data from Debug Port
* @param arr - pointer to array
* @param length - length of array
*/
void DebugPort::Receive(uint8_t* arr, uint8_t& length)
{
	UART1.ReceiveArr(arr, length);
}
