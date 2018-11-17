/**
* @file debug.cpp
* @brief Implementation of Debug Port
*/

#include "uart.hpp"
#include "debug.hpp"
#include "target.hpp"
#include "text.hpp"
#include <string>

extern UART UART1;
DebugPort Debug;


/**
* @brief Init debug port
*/
void DebugPort::Init()
{
	UART1.Init(UART::UART_1);
	RealTime.StartMs(1);
}


/**
* @brief Transmit array of data to Debug Port
* @param arr - pointer to array
* @param length - length of array
*/
void DebugPort::Transmit(const uint8_t* arr, const uint8_t& length)
{
	TransmitHeader(1, 1);
	UART1.TransmitArr(arr, length);
}


/**
* @brief Transmit null-terminated string to Debug Port
* @param str - pointer to null-terminated string
*/
void DebugPort::Transmit(const uint8_t* str)
{
	TransmitHeader(1, 1);
	UART1.TransmitString(str);
}


/**
* @brief Transmit Value to Debug Port
* @param value - value being transmitting
*/
void DebugPort::Transmit(uint32_t value)
{
	TransmitHeader(1, 1);
	uint8_t buffer[8];
	num2str(value, buffer);
	UART1.TransmitString(buffer);
	UART1.TransmitString((uint8_t*)"\n");
}


/**
* @brief Receive array of data from Debug Port
* @param arr - pointer to array
* @param length - length of array
*/
void DebugPort::Receive(uint8_t* arr, uint8_t& length)
{
	TransmitHeader(1, 1);
	UART1.ReceiveArr(arr, length);
}


/**
* @brief Transmit Value to Debug Port
* @param value - value being transmitting
*/
void DebugPort::TransmitHeader(uint8_t port, uint8_t sensor)
{
	uint8_t buffer[8];
	num2str(RealTime.GetElapsedTime()/60, buffer);
	UART1.TransmitString(buffer);
	UART1.TransmitString((uint8_t*)".");
	
	num2str(RealTime.GetElapsedTime()%60, buffer);
	UART1.TransmitString(buffer);
	UART1.TransmitString((uint8_t*)":");
	
	
	if(port == 1)
		UART1.TransmitString((uint8_t*)"Uart1:");
	else
		UART1.TransmitString((uint8_t*)"Uart2:");
	
	if(sensor == 1)
		UART1.TransmitString((uint8_t*)"temperature=:");
	else
		UART1.TransmitString((uint8_t*)"Sensor:");
}
