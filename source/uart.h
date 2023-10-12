
#ifndef __UART_H__
#define __UART_H__
#include "MKL46Z4.h"


void UART_init() ;

void UART_available() ;

void UART_sendBytes(uint8_t byte) ;

}
void UART_sendString(const char* str) ;

uint8_t UART_getByte() ;

uint16_t UART_getBytesUtil(uint8_t* des, uint8_t chr, uint16_t max_len) ;

#endif __UART_H__
