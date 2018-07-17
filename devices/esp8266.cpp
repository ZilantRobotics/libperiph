/**
* @file esp8266.cpp
* @brief Implementation of work with esp8266
*/

#include <esp8266.hpp>
#include <uart.hpp>
#include <soft_timer.hpp>
#include <debug.hpp>
#include <text.hpp>


/// Global and static object's
extern UART UART2;
UART* ESP8266 = &UART2;

extern DebugPort Debug;
static SoftTimer Timer;
WifiEsp8266 Wifi;


/**
* @brief Init ESP8266
*/
void WifiEsp8266::Init()
{
	ESP8266->Init(UART_2);
	uint8_t counterOfCommand = 0;
	
	uint8_t arr[256];
	uint8_t length;
	
	while (1)
	{
		if (Timer.GetStatus() != TIMER_WORKING)
		{
			/// Receive from ESP8266 and transmit to debug
			ESP8266->ReceiveArr(arr, length);
			Debug.Transmit(arr, length);
			
			/// Transmit to ESP8266
			switch(counterOfCommand++)
			{
				case 0:
					/// WiFi mode
					/// Param = 3 => softAP + station mode
					/// This setting will be stored in the flash system parameter area. 
					/// It won�t be erased even when the power is off and restarted.
					ESP8266->TransmitString("AT+CWMODE=3\r\n");
					Timer.StartMs(5000);
					break;
				case 1:
					/// Restart the module
					Send_AT_RST();
					Timer.StartMs(12000);
					break;
				case 2:
					/// Connect to AP 
					/// Param <ssid> string, AP�s SSID 
					/// Param <password> string, MAX: 64 bytes ASCII
					ESP8266->TransmitString("AT+CWJAP=\"ssid\",\"password\"\r\n");
					Timer.StartMs(8000);
					break;
				case 3:
					/// Get local IP address
					ESP8266->TransmitString("AT+CIFSR\r\n");
					Timer.StartMs(4000);
					break;
				case 4:
					/// Establish TCP connection, UDP transmission or SSL connection
					/// Param <type> string, "TCP" or "UDP" 
					/// Param <remote IP> string, remote IP address
					/// Param <remote port> string, remote port number
					ESP8266->TransmitString("AT+CIPSTART=\"TCP\",\"remote IP\",remote port\r\n");
					Timer.StartMs(5000);
					break;
				case 5:
					/// Send data
					/// Param <length> data length, MAX 2048 bytes
					ESP8266->TransmitString("AT+CIPSEND=16\r\n");
					Timer.StartMs(500);
					break;
				case 6:
					/// Data to transmit
					ESP8266->TransmitString("HELLO FROM STM32");
					Timer.StartMs(500);
					break;
				default:
					return;
			}
		}
	}
	
}


/**
* @brief Transmit data to TCP server
*/
template <class T>
void WifiEsp8266::Transmit(const T* ptrArr, const uint8_t& length)
{
	ESP8266->TransmitString("AT+CIPSEND=");				/// Send header
	
	if (length == 4)
	{
		uint8_t str[4];
		num2str(length+2, str);
		ESP8266->TransmitArr(str, 1);					/// Send number of bytes
		ESP8266->TransmitArr("\r\n", 2);				/// Send "\r\n"
		Timer.StartMs(100);
		while(Timer.GetStatus() == TIMER_WORKING);
		ESP8266->TransmitArr(ptrArr, length);			/// Send arr of data
		ESP8266->TransmitString("\r\n");				/// Send arr of data
	}
}
template void WifiEsp8266::Transmit(const uint8_t * ptrArr, const uint8_t& length);
template void WifiEsp8266::Transmit(const char* ptrArr, const uint8_t& length);


/**
* @brief Restart the module
*/
void WifiEsp8266::Send_AT_RST()
{
	ESP8266->TransmitString("AT+RST\r\n");
}


/**
* @brief Get pointer on response array and length of array
* @param ptrArr - pointer on response array
* @param length - link on length of array
*/
template <class T>
void WifiEsp8266::GetResponse(T* ptrArr, uint8_t& length)
{
	
}
template void WifiEsp8266::GetResponse(uint8_t* ptrArr, uint8_t& length);
template void WifiEsp8266::GetResponse(char* ptrArr, uint8_t& length);
