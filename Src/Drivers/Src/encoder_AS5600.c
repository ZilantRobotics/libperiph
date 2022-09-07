/**
 * @file encoder_AS5600.c
 * @author sainquake@gmail.com (ko.burdinov)
 * @date 01.09.2022
 * @note https://ams.com/documents/20143/36005/AS5600_DS000365_5-00.pdf
 * @note https://github.com/nicholasmorrow/AS5600
 * @note https://github.com/Seeed-Studio/Seeed_Arduino_AS5600
 */

#include "encoder_AS5600.h"
#include <string.h>
#include "i2c_manager.h"


// I2C private data:
#define I2C_ID              (AS5600_SLAVE_ADDRESS << 1) + 1
#define I2C_REQUEST_SIZE    1
#define I2C_RESPONSE_SIZE   2
static uint8_t AS5600_tx_buf[I2C_REQUEST_SIZE]  = {0x00};
static uint8_t AS5600_rx_buf[I2C_RESPONSE_SIZE] = {0x00};



void AS5600MeasureCallback() {
    AS5600_tx_buf[0] = AS5600_REGISTER_RAW_ANGLE_HIGH;
    i2cTransmit(I2C_ID, AS5600_tx_buf, I2C_REQUEST_SIZE);
    i2cReceive( I2C_ID, AS5600_rx_buf, I2C_RESPONSE_SIZE);
}

void AS5600FillBuffer(const uint8_t new_buffer[]) {
    memcpy(AS5600_rx_buf, new_buffer, I2C_RESPONSE_SIZE);
}

float AS5600Parse() {
    uint16_t angle = (((uint16_t)AS5600_rx_buf[0]) << 8) + (uint16_t)AS5600_rx_buf[1];

    angle = AS5600_12_BIT_MASK & angle;
    float fangle = ((float)angle)/4095.0f*360.0f;

    return fangle;
}
