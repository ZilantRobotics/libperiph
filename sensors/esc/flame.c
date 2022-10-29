/*
 * Copyright (C) 2018-2021 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file flame.c
 * @author d.ponomarev
 * @date May 27, 2021
 */

#include "flame.h"
#include <string.h>


#define FIRST_BYTE      155
#define SECOND_BYTE     22


typedef struct {
    uint16_t smth;              // 0-1      OK  bytes are: 155, 22
    uint16_t bale_no;           // 2-3      OK  bytes are: 4, 2
    uint16_t weird_counter;     // 4-5      OK  0x??17
    uint16_t rx_pct;            // 6-7      OK  0x0000 -> 0x0004    input pwm scaled from 0->1000
    uint16_t rx_pct_too;        // 8-9      OK  0x2f00 -> 0x0004    input pwm too?
    uint16_t rpm;               // 10-11    OK  0x0000 ->
    uint16_t voltage;           // 12-13    ~OK 0xc?05              30V(1770),25V(1482), coef=59
    uint16_t weird_may_be_crnt;  // 14-15   OK  0x00
    uint16_t zero;              // 16-17    OK  0x00
    uint16_t temperature;       // 18-19    ~OK 0x680c              need calibration
    uint16_t status_code;       // 20-21    OK  0x??0c
    uint16_t verify_code;       // 22-23    OK  0x????
} EscFrame_t;


static uint16_t swap_bytes_order_u16(uint16_t u16);


bool escFlameIsItPackageStart(const uint8_t* raw_package_buffer) {
    return raw_package_buffer[0] == FIRST_BYTE && raw_package_buffer[1] == SECOND_BYTE;
}

void escFlameParse(const uint8_t* raw_package_buffer, EscFlameStatus_t* esc_status) {
    const EscFrame_t* esc_frame = (const EscFrame_t*)raw_package_buffer;
    esc_status->rpm = swap_bytes_order_u16(esc_frame->rpm) * 60.0 / 3.27;
    esc_status->voltage = swap_bytes_order_u16(esc_frame->voltage) / 59.0;
    esc_status->power_rating_pct = swap_bytes_order_u16(esc_frame->rx_pct) * 100.0 / 1024.0;
}

bool escFlameParseDma(uint8_t last_recv_idx,
                      UartDmaParser_t* parser,
                      EscFlameStatus_t* esc_status) {
    static uint8_t auxiliary_buf[ESC_FLAME_PACKAGE_SIZE];
    bool res = false;
    parser->prev_idx = parser->crnt_idx;
    parser->crnt_idx = last_recv_idx;
    if (parser->prev_idx == parser->crnt_idx && parser->prev_idx != parser->saved_idx) {
        uint8_t* package;
        parser->saved_idx = parser->prev_idx;
        if (last_recv_idx < ESC_FLAME_PACKAGE_SIZE - 1) {
            uint16_t first_idx = UART_BUFFER_SIZE - ESC_FLAME_PACKAGE_SIZE + last_recv_idx + 1;
            uint16_t first_package_part_size = ESC_FLAME_PACKAGE_SIZE - last_recv_idx - 1;
            memcpy(auxiliary_buf, &parser->buf[first_idx], first_package_part_size);
            memcpy(auxiliary_buf + first_package_part_size, parser->buf, last_recv_idx + 1);
            package = auxiliary_buf;
        } else {
            uint16_t first_idx = last_recv_idx - ESC_FLAME_PACKAGE_SIZE + 1;
            package = &parser->buf[first_idx];
        }
        if (escFlameIsItPackageStart(package)) {
            escFlameParse(package, esc_status);
            res = true;
        }
    }
    return res;
}

uint16_t swap_bytes_order_u16(uint16_t u16) {
    return (u16 >> 8) + ((u16 & 0xFF) << 8);
}
