#include <queue.h>

void Queue_clear(QueueHandle_t *Queue)
{
	Queue->Count = 0;
    Queue->Front = 0;
    Queue->Rear = 0;
}

void Queue_Init(QueueHandle_t *Queue, uint8_t* buf, uint32_t size)
{
	Queue->QueueArr = buf;
	Queue->Front = -1;
	Queue->Rear = -1;
	Queue->Size = size;
}

uint8_t Queue_Full(QueueHandle_t *Queue)
{
	uint8_t Key = FALSE;
	if((Queue->Front == Queue->Rear + 1) || (Queue->Front == 0 && Queue->Rear == Queue->Size - 1))
	{
		Key = TRUE;
	}
	return Key;
}

uint8_t Queue_Empty(QueueHandle_t *Queue)
{
	uint8_t Key = FALSE;
	if(Queue->Front == -1)
	{
		Key = TRUE;
	}
	return Key;
}

void Queue_Push(QueueHandle_t *Queue, uint8_t InputData)
{
	if (!Queue_Full(Queue))
	{
            if (Queue->Front == -1)
            {
                Queue->Front = 0;
            }

            Queue->Rear = (Queue->Rear + 1) % Queue->Size;

            Queue->QueueArr[Queue->Rear] = InputData;

            Queue->Count++;

	}

}

uint8_t Queue_Pull(QueueHandle_t *Queue, uint8_t *des)
{
    if (Queue->Count == 0)
	{
        return 0;
    }

    *des = Queue->QueueArr[Queue->Front];

    Queue->Front = (Queue->Front + 1) % Queue->Size;

    Queue->Count--;

    return 1;
}

void Queue_PushMulti(QueueHandle_t *Queue, uint8_t *InputData, uint32_t numofbytes)
{
    for (uint32_t i = 0; i < numofbytes; i++)
    {
        if (!Queue_Full(Queue))
        {
            if (Queue->Front == -1)
            {
                Queue->Front = 0;
            }

            Queue->Rear = (Queue->Rear + 1) % Queue->Size;

            Queue->QueueArr[Queue->Rear] = InputData[i];
        }
        else
        {
            break;
        }
    }
}


void Queue_PopMulti(QueueHandle_t *Queue, uint8_t *OutputData, uint32_t numofbytes)
{
    for (uint32_t i = 0; i < numofbytes; i++)
    {
        if (!Queue_Empty(Queue))
        {
            OutputData[i] = Queue->QueueArr[Queue->Rear];
            if (Queue->Front == Queue->Rear)
            {
                Queue->Front = -1;
                Queue->Rear = -1;
            }
            else
            {
                Queue->Rear = (Queue->Rear - 1 + Queue->Size) % Queue->Size;
            }
        }
        else
        {
            break;
        }
    }
}


uint16_t Queue_numOfBytes(QueueHandle_t *Queue)
{
    return (uint16_t)Queue->Count;
}



