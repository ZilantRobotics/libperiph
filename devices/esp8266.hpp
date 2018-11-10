/**
* @file esp8266.hpp
* @brief Class of wi-fi module ESP8266
*/

#ifndef __ESP8266_HPP
#define __ESP8266_HPP

#include "soft_timer.hpp"
#include "stm32f3xx.h"


/**
* @brief Work with wi-fi module ESP8266
*/
class WifiEsp8266
{
	public:
		void Init();
		void GetResponse(uint8_t* ptrArr, uint8_t& length);
		void Transmit(const uint8_t* ptrArr, const uint8_t& length);
	private:
		void Send_AT_RST();
};


#endif	// __ESP8266_HPP
