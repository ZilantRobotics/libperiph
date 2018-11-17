/** 
* @file rangefinder.h
* @brief Declaration of rangefinder
*/

#ifndef RANGEFINDER_H
#define	RANGEFINDER_H

#include <stdint.h>

typedef enum {
    ENCODER_POSITIVE_EDGE = 0,
    ENCODER_NEGATIVE_EDGE,
} InterruptType_t;

void rangefinder_init();
uint16_t rangefinder_do();
uint16_t rangefinder_get_median_range();
void rangefinder_handle_interrupt(InterruptType_t interrupt_type);

#endif	/* RANGEFINDER_H */
