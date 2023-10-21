
#include "uart.h"
#define MAX_BUFFER (100u)

typedef struct
{
	uint8_t arr[MAX_BUFFER] ;
	uint16_t rear ;  	// Point to byte at lastest in
	uint16_t front ;	// Point to byte firstest
	uint16_t count ; 	// Num of byte
}queue_t;

static queue_t recv_queue = {
		.rear = 0 ,
		.front = 0 ,
		.count = 0 ,
};
static queue_t tran_queue = {
		.rear = 0 ,
		.front = 0 ,
		.count = 0 ,
};

volatile uint8_t recv_over_flow_flag = 0 ;

__attribute__ ((section("RamFunction")))
void UART0_IRQHandler()
{
	if((UART0->S1) & UART_S1_RDRF_MASK) // Recv Buffer full
	{
		if(recv_queue.count == MAX_BUFFER)
		{
			recv_over_flow_flag = 1 ;
			__disable_rx_int() ;
		}
		else
		{
			recv_queue.arr[recv_queue.rear] = UART0->D ;

			/* Mod %  operator will call to flash */
//			recv_queue.rear = (recv_queue.rear + 1 ) % MAX_BUFFER ;
			recv_queue.rear ++ ;
			if(recv_queue.rear >= MAX_BUFFER) recv_queue.rear -= MAX_BUFFER ;

			recv_queue.count ++ ;
		}
	}
	if((UART0->S1) & UART_S1_TDRE_MASK)	// Transmit buffer empty
	{
		if(tran_queue.count == 0)
		{
			__disable_tx_int() ;
		}
		else
		{
			UART0->D = tran_queue.arr[tran_queue.front] ;

//			tran_queue.front = (tran_queue.front + 1) % MAX_BUFFER ;
			/* Mod %  operator will call to flash */
			tran_queue.front ++ ;
			if(tran_queue.front >= MAX_BUFFER) tran_queue.front -= MAX_BUFFER ;
			tran_queue.count -- ;
		}
	}
}

void uart_init()
{

	/* Clock initial*/

	// Enables the internal reference clock for use as MCGIRCLK.
	MCG->C2 |= MCG_C2_IRCS_MASK ; // Select fast internal clock source

	MCG->C1 |= MCG_C1_IRCLKEN_MASK ;

	// Fast Clock Internal Reference Divider
	MCG->SC &= ~MCG_SC_FCRDIV_MASK; // Divide by 1

	// Enable clock for UART0
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK ;

	// Select clock source for uart0
	SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK ;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(0b11) ; // MCGIRCLK clock

	/* Baudrate init */

	UART0->BDH &= UART_BDH_SBR_MASK ;

	/* Baudrate = 9600 */
//	UART0->BDL  = 42 ;  // Baudrate = 9600
//	UART0->C4 = (UART0->C4 & ~UART0_C4_OSR_MASK) | UART0_C4_OSR(9) ;

	/* baudrate = 115200 */
	UART0->BDL  = 5 ;
	UART0->C4 = (UART0->C4 & ~UART0_C4_OSR_MASK) | UART0_C4_OSR(6) ;

	/* baudrate = 57600 */
//	UART0->BDL  = 7 ;
//	UART0->C4 = (UART0->C4 & ~UART0_C4_OSR_MASK) | UART0_C4_OSR(9) ;


	/* Data lenght: default 8 bit */

	// Parity
	UART0->C1 &= ~UART_C1_PE_MASK ; // no parity
	UART0->C1 &= ~UART_C1_PT_MASK ;

	// Stop bit
	UART0->BDH &= UART0_BDH_SBNS_MASK ; // one stop bit


	__enable_rx() ;
	__enable_tx() ;
	__enable_rx_int() ;

	NVIC_EnableIRQ(UART0_IRQn) ;
}

void uart_denit()
{
	NVIC_DisableIRQ(UART0_IRQn) ;
	__disable_rx_int() ;
	__disable_tx_int() ;
	__disable_rx() ;
	__disable_tx() ;
	SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK ;
	SIM->SCGC4 &= ~SIM_SCGC4_UART0_MASK ;
	MCG->SC &= ~MCG_SC_FCRDIV_MASK; // Divide by 1
	MCG->SC |= MCG_SC_FCRDIV(0b001) ; // Divide by 2
	MCG->C1 &= ~MCG_C1_IRCLKEN_MASK ;
	MCG->C2 &= ~MCG_C2_IRCS_MASK ;

}

uint8_t uart_sendByte(uint8_t byte)
{
	uint8_t result = 0 ;
	if(tran_queue.count < MAX_BUFFER)
	{
		tran_queue.arr[tran_queue.rear] = byte ;
		tran_queue.rear = ((tran_queue.rear) + 1 ) % MAX_BUFFER ;
		tran_queue.count ++ ;
		__enable_tx_int() ;
	}
	else
	{
		result = -1 ;
	}
	return result ;
}

uint16_t uart_sendUntil(uint8_t* src, uint8_t chr, uint8_t timeout)
{
	uint16_t count = 0 ;
	uint8_t byte ;
	do
	{
		byte = src[count] ;
		if(0 == uart_sendByte(byte))  // Queue is full
		{
			count ++ ;
		}
		else
		{
			if(0 == timeout) break ;
		}
	}
	while(byte != chr) ;

	return count ;
}

int16_t uart_getByte()
{
	int16_t byte ;
	if(recv_queue.count ==  0)
	{
		byte = -1 ;
	}
	else
	{
		byte = recv_queue.arr[recv_queue.front] ;
		recv_queue.front = (recv_queue.front + 1) % MAX_BUFFER ;
		recv_queue.count -- ;
	}
	return byte ;
}

uint16_t uart_getUntil(uint8_t* des, uint8_t chr, uint8_t timeout)
{
	uint16_t count = 0 ;
	int16_t byte ;
	do
	{
		byte = uart_getByte() ;
		if( byte != -1)
		{
			des[count] = byte ;
			count ++ ;
		}
		else {
			if(timeout == 0) break ;
		}
	}
	while(byte != chr) ;

	des[count] = '\0' ; // Add End of string
	return count ;
}

