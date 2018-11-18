/**
 * @file esp8266.hpp
 * @brief Class of wi-fi module ESP8266
 */

#ifndef __ESP8266_HPP
#define __ESP8266_HPP

#include "stdint.h"


/**
 * @brief Work with wi-fi module ESP8266
 */
class WifiEsp8266
{
	public:
		void Init();
		void GetResponse(uint8_t* ptrArr, uint8_t& length);

		/**
		 * @brief Transmit data to TCP server
		 */
		void Transmit(const uint8_t* ptrArr, const uint8_t& length);
	private:
		/**
		 * @brief Restart the module
		 */
		void Send_AT_RST();

		/**
		 * @brief WiFi mode
		 * Param = 3 => softAP + station mode
		 * This setting will be stored in the flash system parameter area.
		 * It wont be erased even when the power is off and restarted.
		 */
		void Send_AT_CWMODE();

		void ConnectToApp();
		void GetLocalIpAddress();
		void EstablishTcpConnection();
		void SendHelloWorld();
};


#endif	// __ESP8266_HPP
