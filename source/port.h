#ifndef __MKL46_PORT_H__
#define __MKL46_PORT_H__


#include "MKL46Z4.h"
#include "core_cm0plus.h"

typedef void (*EXTI_CallbackType)(uint32_t pinnum);

typedef enum {
	PORT_PIN_0	 = 0u,
	PORT_PIN_1,
	PORT_PIN_2,
	PORT_PIN_3,
	PORT_PIN_4,
	PORT_PIN_5,
	PORT_PIN_6,
	PORT_PIN_7,
	PORT_PIN_8,
	PORT_PIN_9,
	PORT_PIN_10,
	PORT_PIN_11,
	PORT_PIN_12,
	PORT_PIN_13,
	PORT_PIN_14,
	PORT_PIN_15,
	PORT_PIN_16,
	PORT_PIN_17,
	PORT_PIN_18,
	PORT_PIN_19,
	PORT_PIN_20,
	PORT_PIN_21,
	PORT_PIN_22,
	PORT_PIN_23,
	PORT_PIN_24,
	PORT_PIN_25,
	PORT_PIN_26,
	PORT_PIN_27,
	PORT_PIN_28,
	PORT_PIN_29,
	PORT_PIN_30,
	PORT_PIN_31,
}PORT_PinNumberType;

typedef enum{
	PORT_SPEED_HIGH		= 0u,
	PORT_SPEED_LOW		= 1u
}PORT_SpeedType;

typedef enum{
	PORT_NOPULL 	= 0u,
	PORT_PULLUP 	= 1u,
	PORT_PULLDOWN	= 2u
}PORT_PULLType;

typedef enum{
	PORT_AFT_0		= 0u,
	PORT_AFT_1		= 1u,
	PORT_AFT_2		= 2u,
	PORT_AFT_3		= 3u,
	PORT_AFT_4		= 4u,
	PORT_AFT_5		= 5u,
	PORT_AFT_6		= 6u,
	PORT_AFT_7		= 7u,
}PORT_ALTNumType;


typedef enum{
	PORT_ALT_ANALOG 	= PORT_AFT_0 ,
	PORT_ALT_GPIO	 	= PORT_AFT_1
}PORT_ALTModeType;

typedef enum{
	PORT_INTERRUPT_DISABLE	 		= 0u,
	PORT_INTERRUPT_DMA_RISING		= 1u,
	PORT_INTERRUPT_DMA_FALLING		= 2u,
	PORT_INTERRUPT_DMA_EITHER		= 3u,
	PORT_INTERRUPT_EXTI_ZERO		= 8u,
	PORT_INTERRUPT_EXTI_RISING		= 9u,
	PORT_INTERRUPT_EXTI_FALLING		= 10u,
	PORT_INTERRUPT_EXTI_EITHER		= 11u,
	PORT_INTERRUPT_EXTI_ONE			= 12u,
}PORT_InterruptType;


typedef struct {
	PORT_ALTModeType ALTMode ;
	PORT_PULLType pull ;
	PORT_SpeedType speed ;
	PORT_InterruptType int_config ;
}PORT_PinConfigType;

void PORT_InitPin(PORT_Type* PORTx, PORT_PinNumberType pinnum, const PORT_PinConfigType* config);

void EXTI_PORTA_SetCallback(EXTI_CallbackType callback);

void EXTI_PORTC_SetCallback(EXTI_CallbackType callback);

void EXTI_PORTD_SetCallback(EXTI_CallbackType callback);





#endif
