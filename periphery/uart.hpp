/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file uart.hpp
 * @brief uart implementation
 * @author d.ponomarev
 * @date Jun 26, 2018
 */

#ifndef __UART_HPP
#define __UART_HPP

#include "stm32f3xx.h"


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
		enum UartNumber_t
		{
			UART_1 = 1,
			UART_2 = 2,
			UART_3 = 3,
			UART_4 = 4,
			UART_5 = 5,
		};
		UART(): IsItInit(false) {};
		void Init(UartNumber_t uartNumber);
		void TransmitString(const uint8_t* str);
		void TransmitArr(const uint8_t* arr, uint8_t length);
		void ReceiveArr(uint8_t* ptrArr, uint8_t& length);
		Buffer BufferRX;
	private:
		void TransmitChar(uint8_t byte);
		bool IsItInit;
		UartNumber_t UartNumber;
		USART_TypeDef* UARTX;
};

#endif //__UART_HPP
