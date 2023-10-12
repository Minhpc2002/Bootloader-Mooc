
#ifndef __UART_H__
#define __UART_H__
#include "MKL46Z4.h"


void UART_Init() ;

void UART_sendBytes(uint8_t byte) ;

void UART_sendString(uint8_t* str, uint8_t len) ;

uint8_t UART_getByte() ;

uint16_t UART_getUtil(uint8_t* des, uint8_t chr, uint16_t max_len) ;

#endif __UART_H__
