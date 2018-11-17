/**
* @file text.cpp
* @brief text functions
*/

#include "text.hpp"


/**
* @brief Translation from num(int32_t) to string
* @param[in] intNum - integer number
* @param[in] str - Ñ-string
* @note It may require no more than 12 bytes bytes (11 bytes - int32_t number, 1 byte - '\n')
*/
void num2str(int32_t intNum, uint8_t* str)
{
	uint8_t byteCount = 0;
    if (intNum < 0)
    {
        intNum = -intNum;
        *str++ = '-';
    }
    if (intNum == 0)
    {
        str[0] = '0';
        str[1] = '\0';
    }
    else
    {
        while(1)
        {
           if (intNum == 0)
              break;
          str[byteCount++] = '0' + intNum%10;
          intNum /= 10;
        }
        str[byteCount] = '\0';
    
    }
    
	// Repositioning the string:
	uint8_t length = byteCount;
	char buf;
    for(byteCount = 0; byteCount < (length >> 1); byteCount++)
    {
        buf = str[length - byteCount - 1];
        str[length - byteCount - 1] = str[byteCount];
        str[byteCount] = buf;
    }
}



///**
//* @brief Translation from num(float) to string
//* @param floatNum - float number
//* @param str - pointer to head of string
//*/
//void num2str(float floatNum, char* str)
//{
//	// Sign "-"
//	if (floatNum < 0)
//    {
//        floatNum = -floatNum;
//        *str++ = '-';
//    }
//	// Main algorithm
//	uint8_t count = 0;
//	char buf;
//    if (floatNum == 0)
//    {
//        str[0] = '0';
//        str[1] = '\0';
//    }
//    else
//    {
//		uint8_t digitsBeforeDot = 0;
//		uint8_t digitsAfterDot = 0;
//		for(uint8_t countSymbol = 0; countSymbol++; countSymbol < MAX_LENGTH_OF_STRING)
//        {
//			
//		}
//        
//        str[count] = '\0';
//    
//    }
//    
//    // Repositioning the string:
//    uint8_t length = count;
//    for(count = 0; count < (length >> 1); count++)
//    {
//        buf = str[length - count - 1];
//        str[length - count - 1] = str[count];
//        str[count] = buf;
//    }
//}
