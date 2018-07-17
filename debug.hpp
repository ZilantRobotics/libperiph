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
			
		template <class T>
		void Transmit(const T* arr, const uint8_t& length);
		
		template <class T>
		void Transmit(const T* str);
		
		template <class T>
		void Receive(T* arr, uint8_t& length);
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
