/**
* @file text.cpp
* @brief text functions
*/

#include <text.hpp>


/**
* @brief Translation from num(int32_t) to string
* @param intNum - integer number
* @param str - pointer to head of string
*/
void num2str(int32_t intNum, char* str)
{
    uint8_t count = 0;
    char buf;
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
          str[count++] = '0' + intNum%10;
          intNum /= 10;
        }
        str[count] = '\0';
    
    }
    
    // Repositioning the string:
    uint8_t length = count;
    for(count = 0; count < (length >> 1); count++)
    {
        buf = str[length - count - 1];
        str[length - count - 1] = str[count];
        str[count] = buf;
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
