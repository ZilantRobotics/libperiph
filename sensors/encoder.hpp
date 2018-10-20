/**
* @file encoder.hpp
* @brief Class of Encoder
*/

#ifndef __ENCODER_HPP
#define __ENCODER_HPP

#include "stm32f3xx.h"

class Encoder
{
	public:
		enum Encoder_t
		{
			LEFT_ENCODER,
			RIGHT_ENCODER,
			MAX_ENCODER_COUNT,
		};
		static Encoder*	GetInstance(Encoder_t);
		uint32_t GetPulses() const;
		void ReserPulses();
	private:
		static Encoder* EncoderLeftPtr;
		static Encoder* EncoderRightPtr;
		Encoder() = delete;
		Encoder(Encoder_t);
		Encoder(const Encoder&) = delete;
        Encoder& operator=(const Encoder&) = delete;
			
		uint32_t* Value;
};


#endif //__ULTRASONIC_RANGEFINDER_HPP
