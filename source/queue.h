#include "MKL46Z4.h"

#define MAX_BUFFER 1024U


typedef struct{
	uint8_t buffer[MAX_BUFFER] ; // Array to store elements
	uint16_t count ; // Current number of element in the queue
	uint16_t front ; // Index of the next element at beginning of the queue
	uint16_t rear ; // Index of the next element to add to the queue
}QueueHandle_t;

/**
 *	@brief reset all parameter of queue to 0
 *	@param Queue pointer to handle struct of queue
 *	@return void
 */
void QUEUE_clear(QueueHandle_t* Queue){
	// set count = 0. front = 0. rear = 0
}

/**
 *	@brief add multi bytes to end ofqueue
 *	@param Queue pointer to handle struct of queue
 *	@param byte pointer to array of bytes to add
 *	@param numofbytes number of bytes to add
 *	@return void
 */
void QUEUE_push(QueueHandle_t * Queue, uint8_t* byte, uint16_t numofbytes){

}

/**
 *	@brief take multi bytes from beginning of queue
 *	@param Queue pointer to handle struct of queue
 *	@param byte pointer to destination
 *	@param numofbytes number of bytes to read
 *	@return number of bytes read success
 */
uint16_t QUEUE_pull(QueueHandle_t * Queue, uint8_t* des, uint16_t numofbytes){

}


uint16_t QUEUE_numOfBytes(QueueHandle_t * Queue){
	return Queue->count ;
}
