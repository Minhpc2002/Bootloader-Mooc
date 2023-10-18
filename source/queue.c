#include <queue.h>

void Queue_clear(QueueHandle_t *Queue)
{
	Queue->Count = 0;
    Queue->Front = -1;
    Queue->Rear = -1;
}

void Queue_init(QueueHandle_t *Queue, uint8_t* buf, uint32_t size)
{
	Queue->QueueArr = buf;
	Queue->Front = -1;
	Queue->Rear = -1;
	Queue->Size = size;
	Queue->Count = 0 ;
}

uint8_t Queue_isFull(QueueHandle_t *Queue)
{
	uint8_t Key = FALSE;
	if((Queue->Front == Queue->Rear + 1) || (Queue->Front == 0 && Queue->Rear == Queue->Size - 1))
	{
		Key = TRUE;
	}
	return Key;
}

uint8_t Queue_isEmpty(QueueHandle_t *Queue)
{
	uint8_t Key = FALSE;
	if(Queue->Front == (uint16_t)-1)
	{
		Key = TRUE;
	}
	return Key;
}

uint8_t Queue_pushByte(QueueHandle_t *Queue, uint8_t byte)
{
	uint8_t isFull = 0 ;

	        if (!Queue_isFull(Queue))
	        {
	            if (Queue->Front == (uint16_t)-1)
	            {
	                Queue->Front = 0;
	            }

	            Queue->Rear = (++ Queue->Rear) % Queue->Size;

	            Queue->Count ++ ;

	        }
	        else
	        {
	        	isFull = 1 ;
	        	Queue->Front = (++ Queue->Front) % Queue->Size;

	        	Queue->Rear = (++ Queue->Rear ) % Queue->Size;

	        }

        	Queue->QueueArr[Queue->Rear] = byte;

	        return isFull ;
}

int16_t Queue_pullByte(QueueHandle_t *Queue)
{
	int16_t result ;
	if(Queue_isEmpty(Queue))
	{
		result = -1 ;
	}
	else
	{
		result = Queue->QueueArr[Queue->Front];

		Queue->Count -- ;

		/* If queue is empty */
		if (Queue->Front == Queue->Rear)
		{
			Queue->Front = -1;
			Queue->Rear = -1;
		}
		else
		{
			Queue->Front = (++ Queue->Front) % Queue->Size;
		}
	}
	return result ;
}


//uint8_t Queue_push(QueueHandle_t *Queue, uint8_t *InputData, uint32_t numofbytes)
//{
//	uint8_t isFull = 0 ;
//    for (uint32_t i = 0; i < numofbytes; i++)
//    {
//        if (!Queue_isFull(Queue))
//        {
//            if (Queue->Front == (uint16_t)-1)
//            {
//                Queue->Front = 0;
//            }
//
//            Queue->Rear = (++ Queue->Rear) % Queue->Size;
//
//            Queue->QueueArr[Queue->Rear] = InputData[i];
//
//            Queue->Count ++ ;
//
//        }
//        else
//        {
//        	isFull = 1 ;
//        	Queue->Front = (++ Queue->Front) % Queue->Size;
//
//        	Queue->Rear = (++ Queue->Rear ) % Queue->Size;
//
//        	Queue->QueueArr[Queue->Rear] = InputData[i];
//        }
//    }
//    return isFull ;
//}


//uint16_t Queue_pull(QueueHandle_t *Queue, uint8_t *OutputData, uint32_t numofbytes)
//{
//	uint16_t count = 0 ;
//    for (uint32_t i = 0; i < numofbytes; i++)
//    {
//        if (!Queue_isEmpty(Queue))
//        {
//            OutputData[i] = Queue->QueueArr[Queue->Front];
//            Queue->Count -- ;
//            count ++ ;
//            if (Queue->Front == Queue->Rear)
//            {
//                Queue->Front = -1;
//                Queue->Rear = -1;
//            }
//            else
//            {
//                Queue->Front = (++ Queue->Front) % Queue->Size;
//            }
//        }
//        else
//        {
//            break;
//        }
//    }
//    return count ;
//}



uint16_t Queue_pullUntil(QueueHandle_t *Queue, uint8_t *OutputData, uint8_t chr)
{
	int16_t byte = -1 ;
	uint16_t i = 0;
	while(byte != (int16_t) chr)
	{
		byte = Queue_pullByte(Queue) ;
		if(byte != -1){
			OutputData[i++] = byte ;
		}
	}
	return i ;
}


uint16_t Queue_numOfBytes(QueueHandle_t *Queue)
{
    return (uint16_t)Queue->Count;
}


