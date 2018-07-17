/**
* @file esp8266.hpp
* @brief Class of wi-fi module ESP8266
*/

#ifndef __ESP8266_HPP
#define __ESP8266_HPP

#include "stm32f3xx.h"
#include "soft_timer.hpp"

/**
* @brief Work with wi-fi module ESP8266
*/
class WifiEsp8266
{
	public:
		void Init();
	
		template <class T>
		void GetResponse(T* ptrArr, uint8_t& length);
		
		template <class T>
		void Transmit(const T* ptrArr, const uint8_t& length);
	private:
		void Send_AT_RST();
};


#endif	// __ESP8266_HPP
