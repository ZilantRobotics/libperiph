/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "flame.h"
#include <string.h>
#include <assert.h>


#define FIRST_BYTE      155
#define SECOND_BYTE     22

#ifdef LIBPERIPH_UNIT_TESTS
    #define LIBPERIPH_STATIC
#else
    #define LIBPERIPH_STATIC static
#endif

typedef struct {
    uint16_t header;            // 0-1      Bytes are: 155, 22
    uint16_t bale_no;           // 2-3
    uint16_t counter;           // 4-5
    uint16_t throttle_in;       // 6-7
    uint16_t throttle_out;      // 8-9
    uint16_t rpm;               // 10-11
    uint16_t voltage;           // 12-13
    uint16_t current;           // 14-15
    uint16_t current_phase;     // 16-17
    uint8_t temperature_1;      // 18-19
    uint8_t temperature_2;      // 18-19
    uint16_t status_code;       // 20-21
    uint16_t crc16;             // 22-23
} EscFlameRaw_t;
static_assert(sizeof(EscFlameRaw_t) == 24, "Wrong size");


static uint8_t temp_buffer[ESC_FLAME_PACKAGE_SIZE];

typedef struct {
    float voltage;
    float rpm;
    float current;
    float throttle;
    uint8_t mot_num_poles;
} Multipliers_t;

// ESC coefficients (ESC Flame by default)
static Multipliers_t config = {
    .voltage = 1.0f / 59.0f,
    .rpm = 60.0f / 3.27f * 28.0f,
    .current = 0,
    .throttle = 100.0f / 1024.0f,
    .mot_num_poles = 28,
};

LIBPERIPH_STATIC bool escFlameIsItPackageStart(const uint8_t* buffer);
LIBPERIPH_STATIC void escFlameParse(const uint8_t* buffer, EscFlame_t* esc_status);
static uint16_t swapBytesU16(uint16_t u16);
static float escRawTemperatureToKelvin(uint8_t raw_temperature);


bool escFlameParseDma(size_t last_idx, DmaUartHandler_t* parser, EscFlame_t* esc_flame) {
    if (parser == NULL || last_idx >= parser->size || esc_flame == NULL) {
        return false;
    }

    bool res = false;
    if (last_idx != parser->saved_idx) {
        const uint8_t* package;
        parser->saved_idx = last_idx;
        if (last_idx < ESC_FLAME_PACKAGE_SIZE - 1) {
            uint16_t first_idx = parser->size - ESC_FLAME_PACKAGE_SIZE + last_idx + 1;
            uint16_t first_package_part_size = ESC_FLAME_PACKAGE_SIZE - last_idx - 1;
            memcpy(temp_buffer, &parser->buf[first_idx], first_package_part_size);
            memcpy(temp_buffer + first_package_part_size, parser->buf, last_idx + 1);
            package = temp_buffer;
        } else {
            uint16_t first_idx = last_idx - ESC_FLAME_PACKAGE_SIZE + 1;
            package = &parser->buf[first_idx];
        }

        if (escFlameIsItPackageStart(package)) {
            escFlameParse(package, esc_flame);
            res = true;
        }
    }
    return res;
}

void escSetAlphaParameters() {
    config.voltage = 1 / 10.0;
    config.rpm = 20.258398f;  // experimental value
    config.current = 0.000015625;
    config.throttle = 100.0 / 1024.0;
    config.mot_num_poles = 28;
}

void escMotorNumberOfPoles(uint8_t mot_num_poles) {
    config.mot_num_poles = (mot_num_poles == 0) ? 1 : mot_num_poles;
}

LIBPERIPH_STATIC bool escFlameIsItPackageStart(const uint8_t* raw_package_buffer) {
    if (raw_package_buffer == NULL) {
        return false;
    }

    return raw_package_buffer[0] == FIRST_BYTE && raw_package_buffer[1] == SECOND_BYTE;
}

LIBPERIPH_STATIC void escFlameParse(const uint8_t* raw_package_buffer, EscFlame_t* esc_flame) {
    if (raw_package_buffer == NULL || esc_flame == NULL) {
        return;
    }

    const EscFlameRaw_t* buffer = (const EscFlameRaw_t*)raw_package_buffer;
    esc_flame->counter = buffer->counter;
    esc_flame->rpm = swapBytesU16(buffer->rpm) * config.rpm / config.mot_num_poles;
    esc_flame->voltage = swapBytesU16(buffer->voltage) * config.voltage;
    esc_flame->current = swapBytesU16(buffer->current) * config.current;
    esc_flame->temperature = escRawTemperatureToKelvin(buffer->temperature_1);
    esc_flame->power_rating_pct = swapBytesU16(buffer->throttle_in) * config.throttle;
}

static uint16_t swapBytesU16(uint16_t u16) {
    return (u16 >> 8) + ((u16 & 0xFF) << 8);
}

/**
  * @note Actual formula is:
  * fahrenheit = 273 - raw_temperature
  * kelvin = (fahrenheit - 32) * 5 / 9 + 273
  * Here is an optimized version
  */
static float escRawTemperatureToKelvin(uint8_t raw_temperature) {
    return (773 - raw_temperature) * 0.5555556f;
}
