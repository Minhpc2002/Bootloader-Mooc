
#include "queue.h"
#include "uart.h"
#include "MKL46Z4.h"

static volatile uint32_t DataRecevie = 0;
static uint8_t FirmReceiveOk = 0;
static uint8_t tempRX;


QueueHandle_t Recv_Queue ;

void UART0_DriverIRQHandler()
{
    /* code */
    FirmReceiveOk = 0;

    // recevie data
    tempRX = UART_getByte();

    // check data
    if(((47 <tempRX) && (tempRX < 58)) || ((64 <tempRX) && (tempRX < 71)))
    {
      // write to Queue
      Queue_Push(&Recv_Queue , tempRX);

      DataRecevie++;
    }
    // check end of file
    else if(tempRX == 'P')
    {
        FirmReceiveOk = 1;
    }

    /* Clear Pending Interrupt */
    NVIC_ClearPendingIRQ(UART0_IRQn);
}

void UART0_Clock2Mhz()
{
	//set IRCS = 4MHz IRC and divide 2
	MCG->C2 &= ~(MCG_C2_IRCS_MASK);            //clear IRCS
	MCG->C2 |= MCG_C2_IRCS(1);                 // set IRCS

	// Enables the internal reference clock for use as MCGIRCLK
	MCG->C1 &= ~MCG_C1_IRCLKEN_MASK;           // clear IRCLKEN
	MCG->C1 |= MCG_C1_IRCLKEN_MASK;            //set IRCLKEN

	/*set UART0_Clock = MCGIRCLK*/
    SIM->SOPT2 = (SIM->SOPT2 & ~SIM_SOPT2_UART0SRC_MASK) | SIM_SOPT2_UART0SRC(3);

    /* Enable clock for PORTA & UART0 */
    SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
    SIM->SCGC4 |= SIM_SCGC4_UART0(1);

}


void UART0_PortInit()
{
	/* Clear PCR_MUX Register */
	PORTA->PCR[UART0_RX_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[UART0_TX_PIN] &= ~PORT_PCR_MUX_MASK;

	/* Set Pin PA1 & PA2 = ALT2 Mode */
	PORTA->PCR[UART0_RX_PIN] |= PORT_PCR_MUX(2);
	PORTA->PCR[UART0_TX_PIN] |= PORT_PCR_MUX(2);

	/* Set up Pull-up for UART Pin */
	PORTA->PCR[UART0_RX_PIN] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);
	PORTA->PCR[UART0_TX_PIN] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);
}


void UART0_SetBaudrate()
{
    /*set Baudrate = 2.000.000 / (29*7) = 9600*/
    /*set OSR + 1 = 11*/
    UART0->C4 = (UART0->C4 & (~UART0_C4_OSR_MASK)) | UART0_C4_OSR(6);

   /*set BR = 33*/
    UART0->BDH &= (~UART0_BDH_SBR_MASK);
    UART0->BDL = (UART0->BDL & (~UART0_BDL_SBR_MASK)) | UART0_BDL_SBR(29);
}


void UART0_SetFrame()
{
    /* Select number of Data Bits - 8 bits */
    UART0->C4 &= (~UART0_C4_M10_MASK);
    UART0->C1 &= (~UART0_C1_M_MASK);

    /* Select Parity Bit - None */
    UART0->C1 &= (~UART0_C1_PE_MASK);

    /*set mode LSB*/
    UART0->S2 &= (~UART0_S2_MSBF_MASK);

   /*set receive data not inverted*/
    UART0->S2 &= (~UART0_S2_RXINV_MASK);

   /*set transmit data not inverted*/
    UART0->C3 &= (~UART0_C3_TXINV_MASK);
}


void UART0_Init()
{
	UART0_Clock2Mhz();
	UART0_PortInit();
	UART0_SetBaudrate();
	UART0_SetFrame();

	/* Enable Transmitter & Receiver */
	UART0->C2 |= UART0_C2_TE(1) | UART0_C2_RE(1);
}

/**
 *	@brief Check to see if there is data to read
 *	@return number of bytes in queue
 */
uint16_t UART_available()
{
	return Queue_numOfBytes(&Recv_Queue) ;
}

void UART_sendBytes(uint8_t byte)
{
    while ((UART0->S1 & UART0_S1_TDRE_MASK));

    UART0->D = byte;
}

void UART_sendString(const char* str)
{
    while (*str)
	{
        UART_sendBytes(*str);
        str++;
    }
}

uint8_t UART_getByte(QueueHandle_t* Recv_Queue)
{
    uint8_t data;

    if((Recv_Queue->Count) == 0)
	{
        return 0;
    }

    if ((Recv_Queue->Count) > 0)
	{
        Queue_Pull(Recv_Queue, &data);
    }
    return data;
}


/**
 *	@brief read from the queue until it reach first character similar to 'chr'
 *	or read up to the maximum number of bytes
 *	@param des pointer to where to save the string
 *	@param chr character to compare
 *	@param max_len maximum number of bytes to read
 *	@return number of bytes read success
 */
void UART_getBytesUtil(uint8_t* des, uint8_t chr, uint16_t max_len)
{
    uint16_t len = 0;
    uint16_t data;
    while (len < max_len)
	{
        while ((UART0->S1 & UART0_S1_RDRF_MASK)== 0);

       data = UART0->D;

        des[len] = data;
        len++;

        if (data == chr)
		{
            break;
        }
    }

    if (len < max_len)
	{
        des[len] = '\0';
    }
}
