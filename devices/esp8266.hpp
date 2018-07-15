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
		void GetResponse(uint8_t* ptrArr, uint8_t& length);
	private:
		void Send_AT();
		void Send_AT_RST();
		void Send_AT_CWSAP();
		void Send_AT_CIPMUX();
		void Send_AT_CIPSERVER();
		void Send_AT_CIFSR();
};


#endif	// __ESP8266_HPP
