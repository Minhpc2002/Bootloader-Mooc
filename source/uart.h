#ifndef __UART_H__
#define __UART_H__


#include "MKL46Z4.h"
#include "core_cm0plus.h"

#define __enable_rx() (UART0->C2 |= UART_C2_RE_MASK)
#define __enable_tx() (UART0->C2 |= UART_C2_TE_MASK)
#define __disable_rx() (UART0->C2 &= ~UART_C2_RE_MASK)
#define __disable_tx() (UART0->C2 &= ~UART_C2_TE_MASK)

#define __enable_rx_int() (UART0->C2 |= UART_C2_RIE_MASK )
#define __enable_tx_int() (UART0->C2 |= UART_C2_TIE_MASK )
#define __disable_rx_int() (UART0->C2 &= ~UART_C2_RIE_MASK )
#define __disable_tx_int() (UART0->C2 &= ~UART_C2_TIE_MASK )


void uart_init() ;
void uart_denit() ;
uint8_t uart_sendByte(uint8_t byte) ;
uint16_t uart_sendUntil(uint8_t* src, uint8_t chr, uint8_t timeout) ;
int16_t uart_getByte() ;
uint16_t uart_getUntil(uint8_t* des, uint8_t chr, uint8_t timeout) ;

#define uart_sendString(string) uart_sendUntil((uint8_t*)(string), '\0', 0) ;
#define uart_getLine(line) uart_getUntil((uint8_t*)(line), '\n', 1) ;

#endif
