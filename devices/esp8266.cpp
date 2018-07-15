/**
* @file esp8266.cpp
* @brief Implementation of work with esp8266
*/

#include <esp8266.hpp>
#include <uart.hpp>
#include <soft_timer.hpp>

extern UART UART2;
UART* ESP8266 = &UART2;

static SoftTimer Timer;
WifiEsp8266 Wifi;



void WifiEsp8266::Init()
{
	ESP8266->Init(UART_2);
	uint8_t counter = 0;
	while (counter != 6)
	{
		if (Timer.GetStatus() != TIMER_WORKING)
		{
			// Timer
			Timer.StartMs(5000);
			switch(counter++)
			{
				case 0:
					Send_AT();
					break;
				case 1:
					Send_AT_RST();
					break;
				case 2:
					Send_AT_CWSAP();
					break;
				case 3:
					Send_AT_CIPMUX();
					break;
				case 4:
					Send_AT_CIPSERVER();
					break;
				case 5:
					Send_AT_CIFSR();
					break;
				default:
					break;
				
			}
			
		}
	}
	
}


void WifiEsp8266::Send_AT()
{
	ESP8266->SendArr("AT\r\n", 4);
}


void WifiEsp8266::Send_AT_RST()
{
	ESP8266->SendArr("AT+RST\r\n", 8);
}


void WifiEsp8266::Send_AT_CWSAP()
{
	ESP8266->SendArr("AT+CWSAP=\"NEX\",\"123456789\",11,0\r\n", 10);
}


void WifiEsp8266::Send_AT_CIPMUX()
{
	ESP8266->SendArr("AT+CIPMUX\r\n", 11);
}


void WifiEsp8266::Send_AT_CIPSERVER()
{
	ESP8266->SendArr("AT+CIPSERVER\r\n", 14);
}


void WifiEsp8266::Send_AT_CIFSR()
{
	ESP8266->SendArr("AT+CIFSR\r\n", 10);
}


/**
* @brief Get pointer on response array and length of array
* @param ptrArr - pointer on response array
* @param length - link on length of array
*/
void WifiEsp8266::GetResponse(uint8_t* ptrArr, uint8_t& length)
{
	ESP8266->GetData(ptrArr, length);
}
