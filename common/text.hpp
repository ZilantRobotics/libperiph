#ifndef __TEXT_HPP
#define __TEXT_HPP

#include "stdint.h"

/**
* @brief Translation from num(int32_t) to string
* @param[in] intNum - integer number
* @param[in] str - �-string
* @note It may require no more than 12 bytes bytes (11 bytes - int32_t number, 1 byte - '\n')
*/
void num2str(int32_t num, uint8_t* str);

#endif //__TEXT_HPP
