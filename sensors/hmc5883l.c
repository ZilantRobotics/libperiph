/**
* @file hmc5883l.c
*/

#include "hmc5883l.h"

void hmc5883l_parse(const uint8_t buf[6], float mag[3]) {
	int16_t raw_data[3];
	raw_data[0] = (buf[0]<<8) + buf[1];
	raw_data[1] = (buf[2] << 8) + buf[3];
	raw_data[2] = (buf[4] << 8) + buf[5];

	mag[0] = raw_data[0]/1090.0;
	mag[1] = raw_data[1]/1090.0;
	mag[2] = raw_data[2]/1090.0;
}
