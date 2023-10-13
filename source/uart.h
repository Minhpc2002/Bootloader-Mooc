
#ifndef __UART_H__
#define __UART_H__
#include "MKL46Z4.h"


void UART_init()
// baud rate == 115200, parity = none , stop bit = 1

void UART_Denit() ;

uint32_t UART_available(){
//	return QUEUE_numOfBytes(Queue) ;
}


void UART_sendBytes(uint8_t byte) ;

}
void UART_sendString(const char* str) ; // a string ended by a '\n'

uint8_t UART_getByte() ;

uint16_t UART_getBytesUtil(uint8_t* des, uint8_t chr, uint16_t max_len) ;

#endif __UART_H__
