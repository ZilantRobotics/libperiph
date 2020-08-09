/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file rm3100.c
 * @author d.ponomarev
 * @note https://www.terraelectronica.ru/pdf/show?pdf_file=%252Fds%252Fpdf%252FR%252FRM3100.pdf
 */

#include "rm3100.h"
#include <stdint.h>

#define I2C_ID      0x20    // The higher 5 bits are pre-defined (0b01000xx)
#define REG_POLL    0x00    // Polls for a Single Measurement
#define REG_CMM     0x01    // Initiates Continuous Measurement Mode
#define REG_TMRC    0x0B    // Sets Continuous Measurement Mode Data Rate
#define REG_MX      0x24    // Measurement Results – X Axis
#define REG_MY      0x27    // Measurement Results – Y Axis
#define REG_MZ      0x2A    // Measurement Results – Z Axis
#define REG_STATUS  0x34    // Status of DRDY
#define REG_HSHAKE  0x35    // Handshake Register

#define PM_X_Y_Z    0x70


#define READ_MASK   0x80    // REG | MASK
#define WRITE_MASK  0x00    // REG | MASK

typedef struct
{
    float mag[3];
} RM3100_t;
RM3100_t RM3100;

