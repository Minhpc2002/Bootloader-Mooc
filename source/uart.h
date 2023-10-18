

#ifndef __UART_H__
#define __UART_H__


#include "MKL46Z4.h"
#include "core_cm0plus.h"
#include "port.h"


void UART_init() ;
void UART_denit() ;
void UART_sendByte(uint8_t byte) ;
void UART_sendString(char* str) ;
uint16_t UART_available() ;
int16_t UART_readByte() ;
uint16_t UART_readUntil(uint8_t* des, uint8_t chr) ;

#endif
