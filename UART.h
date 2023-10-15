#ifndef _UART0_H_
#define _UART0_H_

#include "MKL46Z4.h"

#define UART0_RX_PIN    1
#define UART0_TX_PIN    2
#define BAUD_RATE       115200U

void UART0_IRQHandler() ;

void UART0_Clock2Mhz();

void UART0_PortInit();

void UART0_SetBaudRate(uint32_t clockSource, uint32_t baudRate);

void UART0_SetFrame();

void UART0_Init();

uint16_t UART_available();

void UART_sendBytes(uint8_t byte) ;

void UART_sendString(const char* str);

uint8_t UART_getByte();

void UART_getBytesUtil(uint8_t* des, uint8_t chr, uint16_t max_len); 

void UART0_DeInit();

void Clock_DeInit();

#endif /* _UART0_H_ */


