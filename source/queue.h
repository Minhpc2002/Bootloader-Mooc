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

void Queue_Init(QueueHandle_t *Queue, uint8_t* buf, uint32_t size);

uint8_t Queue_Full( QueueHandle_t *Queue);

uint8_t Queue_Empty( QueueHandle_t *Queue);

void Queue_Push(QueueHandle_t *Queue, uint8_t InputData);

uint8_t Queue_Pull(QueueHandle_t *Queue, uint8_t *des);

void Queue_PushMulti(QueueHandle_t *Queue, uint8_t *InputData, uint32_t numofbytes) ;

void Queue_PopMulti(QueueHandle_t *Queue, uint8_t *OutputData, uint32_t numofbytes);

uint16_t Queue_numOfBytes(QueueHandle_t *Queue);


#endif
