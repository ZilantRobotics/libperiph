#ifndef __TEXT_HPP
#define __TEXT_HPP

#include "stm32f3xx.h"


enum
{
	MAX_LENGTH_OF_STRING = 6,
};


void num2str(int32_t num, char* str);
//void num2str(float floatNum, char* str);


#endif //__TEXT_HPP
