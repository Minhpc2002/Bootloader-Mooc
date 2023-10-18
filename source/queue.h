#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "MKL46Z4.h"

#define TRUE	(1U)
#define FALSE 	(0U)

typedef struct{
	uint8_t *QueueArr ; // Array to store elements
	uint16_t Count ; // Current number of element in the queue
	uint16_t Front ; // Index of the next element at beginning of the queue
	uint16_t Rear ; // Index of the next element to add to the queue
	uint8_t Size;
}QueueHandle_t;



/**
 *	@brief reset all parameter of queue to 0
 *	@param Queue pointer to handle struct of queue
 *	@return void
 */
void Queue_clear(QueueHandle_t* Queue);

void Queue_init(QueueHandle_t *Queue, uint8_t* buf, uint32_t size);

uint8_t Queue_isFull( QueueHandle_t *Queue);

uint8_t Queue_isEmpty( QueueHandle_t *Queue);

uint8_t Queue_pushByte(QueueHandle_t *Queue, uint8_t byte);

int16_t Queue_pullByte(QueueHandle_t *Queue) ;

uint16_t Queue_pullUntil(QueueHandle_t *Queue, uint8_t *OutputData, uint8_t chr);

uint16_t Queue_numOfBytes(QueueHandle_t *Queue);


#endif
