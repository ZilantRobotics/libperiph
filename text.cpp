/**
* @file text.cpp
*/

#include <text.hpp>

void num2str(int32_t num, char* str)
{
    uint8_t count = 0;
    char buf;
    if (num < 0)
    {
        num = -num;
        *str++ = '-';
    }
    if (num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
    }
    else
    {
        while(1)
        {
           if (num == 0)
              break;
          str[count++] = '0' + num%10;
          num /= 10;
        }
        str[count] = '\0';
    
    }
    
    // Перестановка строки:
    uint8_t length = count;
    for(count = 0; count < (length >> 1); count++)
    {
        buf = str[length - count - 1];
        str[length - count - 1] = str[count];
        str[count] = buf;
    }
}
