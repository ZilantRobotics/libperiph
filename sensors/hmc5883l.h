/**
* @file hmc5883l.h
*/

#ifndef INC_HMC5883L_H_
#define INC_HMC5883L_H_

#include <stdint.h>

void hmc5883l_parse(const uint8_t buf[6], float mag[3]);

#endif  // INC_HMC5883L_H_
