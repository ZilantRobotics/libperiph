/**
* @file encoder.cpp
* @brief Implementation of encoder sensor
*/

/*
	Short description of file:
	Guaranteed that max count of this class instance = 2 (left and right encdoers).
	But in future this number can be increase (for example increase motors count).
	In this way we use classic singleton with several instances;
	
	There is no way to do interruptHandler() functions like Encoder class private method.
	We also shoud not do interruptHandler() like static public method, because it
	violates encapsulation principles.
	So it was decided to do static interruptHandler() in source file like C.
*/


#include "encoder.hpp"

/// Some constans
enum Interrupt_t
{
	FALLING_EDGE_INTERRUPT_TYPE,
	GROWING_EDGE_INTERRUPT_TYPE,
};


/// Static class objects and variables:
static uint32_t LeftEncoderCounter = 0;
static uint32_t RightEncoderCounter = 0;
Encoder* Encoder::EncoderLeftPtr;
Encoder* Encoder::EncoderRightPtr;

/// Static functions (=private Encoder's methods)
static void interruptHandler();
static Interrupt_t getLeftEncoderInterruptType();
static Interrupt_t getRightEncoderInterruptType();
inline static void setLeftEncoderGrowingEdgeInterruptType();
inline static void setLeftEncoderFallingEdgeInterruptType();
inline static void setRightEncoderGrowingEdgeInterruptType();
inline static void setRightEncoderFallingEdgeInterruptType();


/**
* @brief Classic singleton GetInstance() method with some additionals
* @param type - encoderType
* @return encoder instance ptr
*/
Encoder* Encoder::GetInstance(Encoder_t type)
{
	if(type == LEFT_ENCODER)
	{
		EXTI->FTSR |= EXTI_FTSR_TR10;					/// enable interupt on the falling front
		EXTI->RTSR &= ~EXTI_RTSR_TR10;					/// disable interupt on the growing front
		EXTI->IMR |= EXTI_IMR_MR10;						/// enable pin interrupt
		SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PD; 	/// interrupt source
		NVIC_EnableIRQ(EXTI15_10_IRQn);					/// Enable interrupt EXTI15_10 in NVIC
		Encoder::EncoderLeftPtr = new Encoder(LEFT_ENCODER);
		return Encoder::EncoderLeftPtr;
	}
	else if(type == RIGHT_ENCODER)
	{
		EXTI->FTSR |= EXTI_FTSR_TR11;					/// enable interupt on the falling front
		EXTI->RTSR |= EXTI_RTSR_TR11;					/// enable interupt on the growing front
		EXTI->IMR |= EXTI_IMR_MR11;						/// enable pin interrupt
		SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PD; 	/// interrupt source
		NVIC_EnableIRQ(EXTI15_10_IRQn);					/// Enable interrupt EXTI15_10 in NVIC
		Encoder::EncoderRightPtr = new Encoder(RIGHT_ENCODER);
		return EncoderRightPtr;
	}
	else
	{
		return nullptr;
	}
}


/**
* @brief Constructor
* @param type
*/
Encoder::Encoder(Encoder_t type)
{
	if(type == LEFT_ENCODER)
	{
		Value = &LeftEncoderCounter;
	}
	else
	{
		Value = &RightEncoderCounter;
	}
}


/**
* @brief 
* @return 
*/
uint32_t Encoder::GetPulses() const
{
	return *Value;
}


/**
* @brief 
*/
void interruptHandler()
{
	/// Encoder left 
	if(EXTI->PR & EXTI_PR_PR10)
	{
		if (getLeftEncoderInterruptType() == FALLING_EDGE_INTERRUPT_TYPE)
		{
			if (GPIOD->IDR & GPIO_IDR_11)
				LeftEncoderCounter++;
			else
				LeftEncoderCounter--;
			setLeftEncoderGrowingEdgeInterruptType();
		}
		else
		{
			if (GPIOD->IDR & GPIO_IDR_11)
				LeftEncoderCounter--;
			else
				LeftEncoderCounter++;
			setLeftEncoderFallingEdgeInterruptType();
		}
		EXTI->PR |= EXTI_PR_PR10;
	}
	/// Encoder right
	else if(EXTI->PR & EXTI_PR_PR12)
	{
		if (getRightEncoderInterruptType() == FALLING_EDGE_INTERRUPT_TYPE)
		{
			if (GPIOD->IDR & GPIO_IDR_13)
				RightEncoderCounter++;
			else
				RightEncoderCounter--;
			setRightEncoderGrowingEdgeInterruptType();
		}
		else
		{
			if (GPIOD->IDR & GPIO_IDR_13)
				RightEncoderCounter--;
			else
				RightEncoderCounter++;
			setRightEncoderFallingEdgeInterruptType();
		}
		EXTI->PR |= EXTI_PR_PR12;
	}
}


/**
* @brief 
* @return 
*/
Interrupt_t getLeftEncoderInterruptType()
{
	if (EXTI->FTSR & EXTI_FTSR_TR10)
		return FALLING_EDGE_INTERRUPT_TYPE;
	return GROWING_EDGE_INTERRUPT_TYPE;
}


/**
* @brief 
* @return 
*/
Interrupt_t getRightEncoderInterruptType()
{
	if (EXTI->FTSR & EXTI_FTSR_TR12)
		return FALLING_EDGE_INTERRUPT_TYPE;
	return GROWING_EDGE_INTERRUPT_TYPE;
}


/**
* @brief 
*/
void setLeftEncoderGrowingEdgeInterruptType()
{
	EXTI->FTSR &= ~EXTI_FTSR_TR10;
	EXTI->RTSR |= EXTI_RTSR_TR10;
}


/**
* @brief 
*/
void setLeftEncoderFallingEdgeInterruptType()
{
	EXTI->FTSR |= EXTI_FTSR_TR10;
	EXTI->RTSR &= ~EXTI_RTSR_TR10;
}


/**
* @brief 
*/
void setRightEncoderGrowingEdgeInterruptType()
{
	EXTI->FTSR &= ~EXTI_FTSR_TR12;
	EXTI->RTSR |= EXTI_RTSR_TR12;
}


/**
* @brief 
*/
void setRightEncoderFallingEdgeInterruptType()
{
	EXTI->FTSR |= EXTI_FTSR_TR12;
	EXTI->RTSR &= ~EXTI_RTSR_TR12;
}


extern "C"
{
	void EXTI15_10_IRQHandler(void)
	{
		interruptHandler();
	}
}
