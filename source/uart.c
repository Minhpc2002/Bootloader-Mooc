
#include "uart.h"
#include "queue.h"
#define MAX_BUFFER_LEN (50u)
static QueueHandle_t rev_queue ;
static QueueHandle_t trans_queue;
static uint8_t rev_buffer[MAX_BUFFER_LEN] ;
static uint8_t trans_buffer[MAX_BUFFER_LEN] ;

void UART0_IRQHandler(){
	if((UART0->S1) & UART_S1_RDRF_MASK)
	{
		Queue_pushByte(&rev_queue, UART0->D) ;
	}
	if((UART0->S1) & UART_S1_TDRE_MASK)
	{
		int16_t send_byte ;

		send_byte = Queue_pullByte(&trans_queue) ;

		if(send_byte != -1)
		{
			UART0->D = send_byte ;
		}
		else
		{
			// Disable transmit interrupt
			UART0->C2 &= ~UART_C2_TIE_MASK ;
		}
	}

}

void UART_init(){
	// Clock Init

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

	// UART Port pin config
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK ; // Enable clock for port A
	PORT_PinConfigType uart_port_config = {
			.ALTMode = PORT_AFT_2,
			.pull = PORT_PULLUP,
	};
	PORT_InitPin(PORTA, 1, &uart_port_config) ;
	PORT_InitPin(PORTA, 2, &uart_port_config) ;

		// baudrate init

		UART0->BDH &= UART_BDH_SBR_MASK ;

		/* Baudrate = 9600 */
		UART0->BDL  = 42 ;  // Baudrate = 9600
		UART0->C4 = (UART0->C4 & ~UART0_C4_OSR_MASK) | UART0_C4_OSR(9) ;

		/* baudrate = 115200 */
//		UART0->BDL  = 5 ;
//		UART0->C4 = (UART0->C4 & ~UART0_C4_OSR_MASK) | UART0_C4_OSR(6) ;

		// Data lenght: default 8 bit

		// Parity
		UART0->C1 &= ~UART_C1_PE_MASK ; // no parity
		UART0->C1 &= ~UART_C1_PT_MASK ;

		// Stop bit
		UART0->BDH &= UART0_BDH_SBNS_MASK ; // one stop bit

		// Enable transmiter
		UART0->C2 |= UART_C2_TE_MASK ;

		// Enable receicer
		UART0->C2 |= UART_C2_RE_MASK ;

		// Enable receiver interrupt
		UART0->C2 |= UART_C2_RIE_MASK ;


		NVIC_EnableIRQ(UART0_IRQn) ;

	Queue_init(&rev_queue, rev_buffer, MAX_BUFFER_LEN) ;
	Queue_init(&trans_queue, trans_buffer, MAX_BUFFER_LEN) ;

}

void UART_denit()
{
	// Disable receicer
	UART0->C2 &= ~UART_C2_RE_MASK ;
	// Disable receiver interrupt
	UART0->C2 &= ~UART_C2_RIE_MASK ;
	// Disable transmiter
	UART0->C2 &= ~UART_C2_TE_MASK ;

	NVIC_DisableIRQ(UART0_IRQn) ;
}


void UART_sendByte(uint8_t byte){

	Queue_pushByte(&trans_queue, byte) ;

	// Enable transmit interrupt
	UART0->C2 |= UART_C2_TIE_MASK ;

	// Send

}

void UART_sendString(char* str){

	uint32_t i = 0;

	while( str[i] != '\0' )
	{
		Queue_pushByte(&trans_queue, str[i++]) ;
	}
	// Enable transmit interrupt
	UART0->C2 |= UART_C2_TIE_MASK ;
}

uint16_t UART_available(){
	return Queue_numOfBytes(&rev_queue) ;
}

int16_t UART_readByte(){
	int16_t byte = -1;
	while(byte == -1)
	{
		byte = Queue_pullByte(&rev_queue) ;
	}
	return byte ;
}

uint16_t UART_readUntil(uint8_t* des, uint8_t chr)
{
	return Queue_pullUntil(&rev_queue, des, chr) ;
}


