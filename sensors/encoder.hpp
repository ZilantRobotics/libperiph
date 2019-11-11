/**
* @file encoder.hpp
* @brief Encoder class
*/
#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <stdint.h>

enum EncoderPin
{
    LEFT_ENC_A_CH,
    LEFT_ENC_B_CH,
    RIGHT_ENC_A_CH,
    RIGHT_ENC_B_CH,
};

/**
* @brief Encoder
* @details There are few requirements and nuance:
* For incrementing and decrementing encoders values we use:
* - EXT (external interrupts with callbacks)
* For calculating encoders speed (number of impulses which we received for last
* 0.1 second) we use:
* We use following GPIO:
* - PE_10  - left encoder A
* - PE_12  - left encoder B
* - PE_14  - right encoder A
* - PE_15  - right encoder B
*/
class Encoder
{
public:
    static void Reset();
    static int32_t GetLeftValue();
    static int32_t GetRightValue();
    static int32_t GetLeftSpeed();
    static int32_t GetRightSpeed();
};

#endif /* ENCODER_HPP */
