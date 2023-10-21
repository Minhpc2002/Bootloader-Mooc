

#include <port.h>
#include "MKL46Z4.h"
#define NULL 0

static EXTI_CallbackType s_gEXTI_PORTA_Callback = NULL;
static EXTI_CallbackType s_gEXTI_PORTC_Callback = NULL;
static EXTI_CallbackType s_gEXTI_PORTD_Callback = NULL;

__STATIC_INLINE void PORT_EnableClock(PORT_Type* PORTx){
	if(PORTA == PORTx){
		SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK ;
	}
	else if(PORTB == PORTx){
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK ;
	}
	else if(PORTC == PORTx){
		SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK ;
	}
	else if(PORTD == PORTx){
		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK ;
	}
	else if(PORTE == PORTx){
		SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK ;
	}
}

__STATIC_INLINE void PORT_DisableClock(PORT_Type* PORTx){
	if(PORTA == PORTx){
		SIM->SCGC5 &= ~SIM_SCGC5_PORTA_MASK ;
	}
	else if(PORTB == PORTx){
		SIM->SCGC5 &= ~SIM_SCGC5_PORTB_MASK ;
	}
	else if(PORTC == PORTx){
		SIM->SCGC5 &= ~SIM_SCGC5_PORTC_MASK ;
	}
	else if(PORTD == PORTx){
		SIM->SCGC5 &= ~SIM_SCGC5_PORTD_MASK ;
	}
	else if(PORTE == PORTx){
		SIM->SCGC5 &= ~SIM_SCGC5_PORTE_MASK ;
	}
}

__STATIC_INLINE void PORT_SetAlternateMode(PORT_Type* PORTx,PORT_PinNumberType pinnum, PORT_ALTModeType ALTMode){
	PORTx->PCR[(uint8_t)pinnum] &= ~PORT_PCR_MUX_MASK ; // Clear MUX field
	PORTx->PCR[(uint8_t)pinnum] |= PORT_PCR_MUX((uint8_t)ALTMode) ;
}
__STATIC_INLINE void PORT_SetPullMode(PORT_Type* PORTx, PORT_PinNumberType pinnum, PORT_PULLType pull){
	if(PORT_NOPULL != pull){
			PORTx->PCR[(uint8_t)pinnum] |= PORT_PCR_PE(1u) ; // Enable pull
			if(PORT_PULLUP == pull){
				PORTx->PCR[(uint8_t)pinnum] |= PORT_PCR_PS_MASK ;
			}
			else if(PORT_PULLDOWN == pull){
				PORTx->PCR[(uint8_t)pinnum] &= ~PORT_PCR_PS_MASK ;
			}
		}
		else{
			PORTx->PCR[(uint8_t)pinnum] &= ~PORT_PCR_PE_MASK ;
		}
}

__STATIC_INLINE void PORT_SetSlewRate(PORT_Type* PORTx, PORT_PinNumberType pinnum, PORT_SpeedType slr){
	PORTx->PCR[(uint8_t)pinnum] &= ~PORT_PCR_SRE_MASK ;
	PORTx->PCR[(uint8_t)pinnum] |= PORT_PCR_SRE((uint8_t) slr);
}
__STATIC_INLINE void PORT_SetInterruptMode(PORT_Type* PORTx, PORT_PinNumberType pinnum, PORT_InterruptType interrupt){
	PORTx->PCR[(uint8_t)pinnum] &= ~PORT_PCR_IRQC_MASK ;
	PORTx->PCR[(uint8_t)pinnum] |= PORT_PCR_IRQC((uint8_t) interrupt);
}



void PORT_InitPin(PORT_Type* PORTx, PORT_PinNumberType pinnum, const PORT_PinConfigType* config){
	// Enable clock for port
	PORT_EnableClock(PORTx) ;
	// Select alternal function
	PORT_SetAlternateMode(PORTx,pinnum, config->ALTMode) ;
	// Select Pull resistor
	PORT_SetPullMode(PORTx, pinnum, config->pull) ;
	// Select slew rate of output
	PORT_SetSlewRate(PORTx, pinnum, config->speed) ;
	// Config interrupt on digital pin
	if(PORT_ALT_ANALOG != config->ALTMode){
		PORT_SetInterruptMode(PORTx, pinnum, config->int_config) ;
	}
	// Enable interrupt
	if (PORT_INTERRUPT_DISABLE != config->int_config)
		{
			if (PORTx == PORTA)
			{
				NVIC_EnableIRQ(PORTA_IRQn);
			}
			if (PORTx == PORTC || PORTx == PORTD)
			{
				NVIC_EnableIRQ(PORTC_PORTD_IRQn);
			}
		}
}

void PORT_DenitPin(PORT_Type* PORTx, PORT_PinNumberType pinnum)
{
	if (PORTx == PORTA)
	{
		NVIC_DisableIRQ(PORTA_IRQn);
	}
	if (PORTx == PORTC || PORTx == PORTD)
	{
		NVIC_DisableIRQ(PORTC_PORTD_IRQn);
	}

	PORT_SetAlternateMode(PORTx,pinnum, PORT_AFT_0) ;
	// Select Pull resistor
	PORT_SetPullMode(PORTx, pinnum, PORT_NOPULL) ;
	// Select slew rate of output
	PORT_SetSlewRate(PORTx, pinnum, PORT_SPEED_LOW) ;
	// Disable interrupt
	PORT_SetInterruptMode(PORTx, pinnum, PORT_INTERRUPT_DISABLE) ;

}

void EXTI_PORTA_SetCallback(EXTI_CallbackType callback){
	s_gEXTI_PORTA_Callback = callback ;
}
void EXTI_PORTC_SetCallback(EXTI_CallbackType callback){
	s_gEXTI_PORTC_Callback = callback ;
}
void EXTI_PORTD_SetCallback(EXTI_CallbackType callback){
	s_gEXTI_PORTD_Callback = callback ;
}

__STATIC_INLINE uint8_t getPinIntFlag(PORT_Type* PORTx, PORT_PinNumberType pinnum){
	if (PORTx->ISFR & 1 << (uint8_t)pinnum) return 1 ;
	else return 0 ;
}
__STATIC_INLINE void clearPinIntFlag(PORT_Type* PORTx, PORT_PinNumberType pinnum){
	PORTx->PCR[(uint8_t)pinnum] |= PORT_PCR_ISF_MASK;
}
void PORTA_IRQHandler(void){
	for(int i = 0; i < 32; i ++){
		if(getPinIntFlag(PORTA, i) ){
			clearPinIntFlag(PORTA, i) ;
			if(NULL != s_gEXTI_PORTA_Callback ){
				s_gEXTI_PORTA_Callback(i) ;
			}
		}
	}
}
void PORTC_PORTD_IRQHandler(void){

	for(int i = 0; i < 32; i ++){
		if(getPinIntFlag(PORTC, i) ){
			clearPinIntFlag(PORTC, i) ;
			if(NULL != s_gEXTI_PORTC_Callback ){
				s_gEXTI_PORTC_Callback(i) ;
			}
		}
	}

	for(int i = 0; i < 32; i ++){
		if(getPinIntFlag(PORTD, i) ){
			clearPinIntFlag(PORTD, i) ;
			if(NULL != s_gEXTI_PORTD_Callback ){
				s_gEXTI_PORTD_Callback(i) ;
			}
		}
	}
}

