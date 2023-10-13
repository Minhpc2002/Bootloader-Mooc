
#include <gpio.h>

__STATIC_INLINE void GPIO_SetDirection(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum, GPIO_DirectionType dir){
	if(GPIO_INPUT == dir){
		GPIOx->PDDR &= ~(1 << (uint32_t)pinnum);
	}
	else if(GPIO_OUTPUT == dir){
		GPIOx->PDDR |=  (1 << (uint32_t)pinnum);
	}
}
__STATIC_INLINE void GPIO_SetPinMode(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum, GPIO_DirectionType mode){
	if(GPIO_INPUT == mode) GPIOx->PDDR &= ~(1 << (uint8_t)pinnum) ;
	else if(GPIO_OUTPUT == mode) GPIOx->PDDR |= 1 << (uint8_t) pinnum ;
}

void GPIO_InitPin(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum, GPIO_PinConfigType* config ){
	// Set direction
	GPIO_SetPinMode(GPIOx, pinnum, config->direction) ;
	if(config->direction == GPIO_OUTPUT) GPIO_WritePin(GPIOx, pinnum, config->state) ;
}

void GPIO_WritePin(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum, GPIO_PinState state){
	if(GPIO_PIN_SET == state) GPIOx->PDOR |= 1 << pinnum ;
	else if(GPIO_PIN_RESET == state) GPIOx->PDOR &= ~(1 << pinnum) ;
}
void GPIO_TogglePin(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum){
	GPIOx->PTOR |= 1 << pinnum ;
}
GPIO_PinState GPIO_ReadPin(GPIO_Type* GPIOx, GPIO_PinNumberType pinnum){
	if(GPIOx->PDIR & ( 1 << pinnum)) return GPIO_PIN_SET ;
	else return GPIO_PIN_RESET ;
}

