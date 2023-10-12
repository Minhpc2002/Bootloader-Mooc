
#include "queue.h"

QueueHandle_t Recv_Queue ;

void UART0_IRQHandler(){
	// Read data from uart
	//data = UDR
	// Send data to queue
	QUEUE_pushAByte(&UART_Queue) ;
}


void UART_init(){
	// something
	// clear queue
}

/**
 *	@brief Check to see if there is data to read
 *	@return number of bytes in queue
 */
uint16_t UART_available(){
	return QUEUE_numOfBytes(&Recv_Queue) ;
}

void UART_sendBytes(){
	// something
}
void UART_sendString(const char* str){
	// something
}
void UART_getByte(){
	//Queue_pull() ;
}


/**
 *	@brief read from the queue until it reach first character similar to 'chr'
 *	or read up to the maximum number of bytes
 *	@param des pointer to where to save the string
 *	@param chr character to compare
 *	@param max_len maximum number of bytes to read
 *	@return number of bytes read success
 */
void UART_getBytesUtil(uint8_t* des, uint8_t chr, uint16_t max_len){

}
