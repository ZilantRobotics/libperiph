/**
* @file debug.hpp
* @brief Class of DebugPort
*/

#ifndef __DEBUG_HPP
#define __DEBUG_HPP

#include "stm32f3xx.h"

class DebugPort
{
	public:
		DebugPort(): IsItInit(DEBUG_IS_NOT_INITIALIZED) {};
		void Init();
		void Transmit(const uint8_t* arr, const uint8_t& length);
		void Transmit(const uint8_t* str);
		void Transmit(uint32_t value);
		void Receive(uint8_t* arr, uint8_t& length);
	private:
		enum: uint8_t 
		{
			// IsItInit
			DEBUG_IS_NOT_INITIALIZED = 0,
			DEBUG_IS_INITIALIZED = 1,
		};
		uint8_t IsItInit;
};


#endif //__DEBUG_HPP
