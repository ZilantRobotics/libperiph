/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file rangefinder/srf_05.h
 * @author d.ponomarev
 * @brief Implementation of rangefinder
 * @date Nov 17, 2018
 */

#ifndef RANGEFINDER_SRF_05_H_
#define	RANGEFINDER_SRF_05_H_

#include <stdint.h>

typedef enum {
    ENCODER_POSITIVE_EDGE = 0,
    ENCODER_NEGATIVE_EDGE,
} InterruptType_t;

void rangefinder_init();
uint16_t rangefinder_do();
uint16_t rangefinder_get_median_range();
void rangefinder_handle_interrupt(InterruptType_t interrupt_type);

#endif  // RANGEFINDER_SRF_05_H_
