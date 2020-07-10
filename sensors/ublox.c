/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file gps_ublox.c
 * @author d.ponomarev
 */

#include "ublox.h"
#include <stddef.h>


#define SYNC_CHAR_1_CODE        0xB5    // 181
#define SYNC_CHAR_2_CODE        0x62    // 98

#define MSG_NAV_PVT 0x01
#define MSG_NAV_SOL 0x00


typedef enum {
    STATE_SYNC_CHAR_1,
    STATE_SYNC_CHAR_2,
    STATE_CLASS,
    STATE_ID,
    STATE_LENGTH_1,
    STATE_LENGTH_2,
    STATE_PAYLOAD,
    STATE_CHECKSUM_A,
    STATE_CHECKSUM_B,
} UbloxState_t;

typedef enum {
    CLASS_NAV = 0x01,
} UbloxClass_t;

typedef enum {
   ID_UNKNOWN = 0,
} UbloxId_t;

typedef struct{
    uint8_t crc_a;
    uint8_t crc_b;
} UbloxCrcChecker_t;

typedef struct{
    UbloxState_t state;
    UbloxClass_t class;
    UbloxId_t id;
    uint16_t length;
    uint8_t payload[128];
    uint16_t crc;
    uint8_t payload_counter;
    UbloxCrcChecker_t crc_checker;
} UbloxPackage_t;
static UbloxPackage_t package = {0};

static int8_t crc_perform_check();
static void crc_clear();
static void crc_add_byte(uint8_t byte);
static void crc_add_arr(const uint8_t* buf, size_t size);


void ubloxNextByte(uint8_t byte){
    switch (package.state)
    {
    case STATE_SYNC_CHAR_1:
        if(byte == SYNC_CHAR_1_CODE){
            package.state = STATE_SYNC_CHAR_2;
        }
        break;
    case STATE_SYNC_CHAR_2:
        package.state = (byte == SYNC_CHAR_2_CODE) ? STATE_CLASS : STATE_SYNC_CHAR_1;
        break;
    case STATE_CLASS:
        package.class = byte;
        package.state = (byte == CLASS_NAV) ? STATE_ID : STATE_SYNC_CHAR_1;
        break;
    case STATE_ID:
        package.id = byte;
        package.state = STATE_LENGTH_1;
        break;
    case STATE_LENGTH_1:
        package.length = byte;
        package.state = STATE_LENGTH_2;
        break;
    case STATE_LENGTH_2:
        package.length += (byte << 8);
        package.state = STATE_PAYLOAD;
        package.payload_counter = 0;
        break;
    case STATE_PAYLOAD:
        if(package.payload_counter + 1 >= package.length){
            package.state = STATE_CHECKSUM_A;
        }
        package.payload[package.payload_counter++] = byte;
        break;
    case STATE_CHECKSUM_A:
        package.crc = byte;
        package.state = STATE_CHECKSUM_B;
        break;
    case STATE_CHECKSUM_B:
        package.crc += (byte << 8);
        package.state = STATE_SYNC_CHAR_1;
        crc_perform_check();
        break;
    default:
        break;
    }
}


int8_t crc_perform_check(){
    crc_clear();
    crc_add_byte(package.class);
    crc_add_byte(package.id);
    crc_add_byte(package.length & 0xFF);
    crc_add_byte((package.length >> 8) && 0xFF);
    crc_add_arr(package.payload, package.length);

    uint16_t real_crc = package.crc;
    uint16_t expected_crc = package.crc_checker.crc_a + (package.crc_checker.crc_b << 8);
    if(real_crc == expected_crc){
        return 0;
    }
    return -1;
}

void crc_clear(){
    package.crc_checker.crc_a = 0;
    package.crc_checker.crc_b = 0;
}

void crc_add_byte(uint8_t byte){
    package.crc_checker.crc_a += byte;
    package.crc_checker.crc_b += package.crc_checker.crc_a;
}

void crc_add_arr(const uint8_t* buf, size_t size){
    while(size-- != 0){
        crc_add_byte(*buf++);
    }
}
