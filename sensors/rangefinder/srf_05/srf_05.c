/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file srf_05.c
 * @author d.ponomarev
 * @brief Implementation of rangefinder
 * @date Nov 17, 2018
 */

#include "srf_05.h"
#include "stdbool.h"


static uint32_t range;
static uint32_t lastTime;

void timer_wait_ms(uint16_t ms);
void timer_wait_us(uint16_t us);
void rangefinder_set_trig_pin(bool trig_value);
uint32_t get_time_now();

static void rangefinder_give_impulse();
static uint16_t rangefinder_get_range();

void rangefinder_init() {
    range = 0;
}


uint16_t rangefinder_do() {
    rangefinder_give_impulse();
    timer_wait_ms(100);
    return rangefinder_get_range();
}

void rangefinder_give_impulse() {
    rangefinder_set_trig_pin(false);
    timer_wait_us(5);

    rangefinder_set_trig_pin(true);
    timer_wait_us(10);
    rangefinder_set_trig_pin(false);
}


uint16_t rangefinder_get_range() {
    return (float)range*0.0085 + 1.43;
}


uint16_t rangefinder_get_median_range() {
    uint16_t range_1 = rangefinder_do();
    uint16_t range_2 = rangefinder_do();
    uint16_t range_3 = rangefinder_do();

    if (range_1 >= range_2) {
        if (range_1 >= range_3)
            return (range_2 >= range_3) ? range_2 : range_3;
        else
            return range_1;
    } else {
        if (range_2 >= range_3)
            return (range_1 > range_3) ? range_1 : range_3;
    }

    return range_2;
}

void rangefinder_handle_interrupt(InterruptType_t interrupt_type) {
    uint32_t nowTime = get_time_now();

    if (interrupt_type == ENCODER_POSITIVE_EDGE) {
        lastTime = nowTime;
    } else {
        range = nowTime - lastTime;
    }
}
