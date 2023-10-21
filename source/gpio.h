#ifndef __MKL46_GPIO_h__
#define __MKL46_GPIO_h__

#include "MKL46Z4.h"
#include "core_cm0plus.h"
typedef enum{
	GPIO_PIN_0 = 0u,
	GPIO_PIN_1 ,
	GPIO_PIN_2 ,
	GPIO_PIN_3 ,
	GPIO_PIN_4 ,
	GPIO_PIN_5 ,
	GPIO_PIN_6 ,
	GPIO_PIN_7 ,
	GPIO_PIN_8 ,
	GPIO_PIN_9 ,
	GPIO_PIN_10 ,
	GPIO_PIN_11 ,
	GPIO_PIN_12 ,
	GPIO_PIN_13	,
	GPIO_PIN_14 ,
	GPIO_PIN_15 ,
	GPIO_PIN_16 ,
	GPIO_PIN_17 ,
	GPIO_PIN_18 ,
	GPIO_PIN_19 ,
	GPIO_PIN_20 ,
	GPIO_PIN_21	,
	GPIO_PIN_22 ,
	GPIO_PIN_23 ,
	GPIO_PIN_24 ,
	GPIO_PIN_25 ,
	GPIO_PIN_26 ,
	GPIO_PIN_27 ,
	GPIO_PIN_28	,
	GPIO_PIN_29 ,
	GPIO_PIN_30 ,
	GPIO_PIN_31 ,
}GPIO_PinNumberType;

typedef enum {
	GPIO_INPUT 		= 0u,
	GPIO_OUTPUT		= 1u
}GPIO_DirectionType;


typedef enum
{
  GPIO_PIN_RESET 	= 0u,
  GPIO_PIN_SET 		= 1u
} GPIO_PinState;

typedef struct{
	GPIO_DirectionType direction ;
	GPIO_PinState state ;
}GPIO_PinConfigType;

void GPIO_InitPin(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum, GPIO_PinConfigType* config ) ;
void GPIO_DenitPin(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum) ;
void GPIO_WritePin(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum, GPIO_PinState state) ;
void GPIO_TogglePin(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum) ;
GPIO_PinState GPIO_ReadPin(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum) ;





#endif
