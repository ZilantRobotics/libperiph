#ifndef __TARGET_HPP
#define __TARGET_HPP

#include "stm32f3xx.h"

/**
* @brief Äנאיגונ ןכאעû
*/
class TargetBase
{
	private:
		enum
		{
			GPIO_Mode_IN = 0,	///< Input Mode
			GPIO_Mode_OUT = 1,	///< Output Mode 
			GPIO_Mode_AF = 2,	///< Alternate function Mode 
			GPIO_Mode_AN = 3,	///< Analog Mode 
		};
	public:
		enum Led_t
		{
			LD4 =  8,
			LD3 =  9,	///< North
			LD5 =  10,
			LD7 =  11,	///< East
			LD9 =  12,
			LD10 = 13,	///< South
			LD8 =  14,
			LD6 =  15,	///< West
			LD_MIN = 8,
			LD_MAX = 15,
		};
		void InitGPIO();
		void LedOn(uint8_t ledNumber);
		void LedOff(uint8_t ledNumber);
};


#endif //__TARGET_HPP
