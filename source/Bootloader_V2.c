
#include "main.h"


extern void (* const g_pfnVectors[])(void) ;
extern uint32_t __base_RAM ;
extern uint32_t __top_RAM ;
extern uint32_t __top_Flash ;

void (* copy[47])(void) __attribute__((section("relo_vector"))) ;


/**************************** Main function *************************/
void main()
{
	system_init() ;

	// Check boot pin level
	if(GPIO_PIN_RESET == GPIO_ReadPin(GPIOC, GPIO_PIN_3)){
	// Bootloader mode

		LOG(BLD_MESSAGE, "\n********* Bootloader V2 ***********\n \
				"		 "		  Entered bootloader! \n") ;

		uint8_t buffer[100] ;
		uint8_t ready_to_flash = 0 ;
		BootloaderStatus_t status = BLD_OK ;
		uint8_t start_sector ;
		uint8_t num_of_sectors ;
		char temp[100] ;

		// Wait for data to arrive
		while(1){
			// Read a line from uart buffer
			status = BLD_OK ;

			uart_getLine(buffer) ;

			switch(buffer[0]){
				/* Load new firmware */
				case 'L':
					// take sector number
					status = hex_to_byte(&start_sector, buffer + 1) ;

					if(BLD_OK == status)
					{
						status = hex_to_byte(&num_of_sectors, buffer + 3) ;

						if(BLD_OK == status)
						{
							if(FLASH_OK == Flash_checkSectorsErased(start_sector, num_of_sectors))
							{
								// Ready to flash
								status = BLD_OK ;
								ready_to_flash = 1 ;
								LOG(BLD_MESSAGE, "Readdy to flash. Waitting....\n") ;
							}
							else{
								status = BLD_HAVE_NOT_ERASE_YET ;
							}
						}
					}
					break ;
				/* Reverse to other sector  */
				case 'R':

					status = hex_to_byte(&start_sector, buffer + 1) ;

					if(BLD_OK == status)
					{
						Flash_eraseSector(BOOT_WORD_ADDRESS / 1024) ;

						Flash_writeWord(BOOT_WORD_ADDRESS, start_sector) ;

						sprintf(temp, "Reversed to previous firmware at sector 0x%x\n", start_sector) ;

						LOG(BLD_MESSAGE, temp) ;
					}
					break ;
				/* Erase sectors */
				case 'E':

					// take sector number...
					status = hex_to_byte(&start_sector, buffer + 1) ;
					if(BLD_OK == status)
					{
						//Take num of sector to erase
						status = hex_to_byte(&num_of_sectors, buffer + 3) ;

						if(BLD_OK == status)
						{

							if(FLASH_OK == Flash_eraseMultiSectors(start_sector, num_of_sectors ))
							{
								status = BLD_OK ;
								sprintf(temp, "Erased at %d sector from 0x%02x\n", num_of_sectors,  start_sector) ;
								LOG(BLD_MESSAGE, temp) ;
							}
							else{
								status = BLD_ERR_ERASED ;
							}
						}
					}
					break;
				/* Write firmware */
				case 'S':
					if(ready_to_flash == 1)
					{
						SREC_t data;

						if(SREC_OK == SREC_parse(&data, buffer))
						{
							if(S1 == data.type || S2 == data.type || S3 == data.type)
							{

								for(int i = 0; i < data.byte_count ; i += 4){
									// Write to flash memory

									if(FLASH_OK != Flash_writeWord(data.address + i, *((uint32_t*)(data.data + i))))
									{
										status = BLD_FLASH_ERR ;
										ready_to_flash = 0 ;
										break;
									}
									else {
										status = BLD_OK ;
									}
								}
							}
							if(S9 == data.type || S8 == data.type || S7 == data.type)
							{
								LOG(BLD_MESSAGE, "Flash finished!\n") ;
								status = BLD_OK ;
								ready_to_flash = 0 ;
							}
						}
						else{
							ready_to_flash = 0 ;
							status = BLD_ERR_DATA ;
						}
					}
					else
					{
						status = BLD_HAVE_NOT_ERASE_YET ;
					}
					break ;

				case 'T':
					__NVIC_SystemReset() ;
					break;
				default:
					status = BLD_SYNTAX_ERR ;
			}

			LOG(status, NULL) ;

		}
	}
	else{

		/* Jump to Application code */
		uint32_t bootsector = Flash_readWord(BOOT_WORD_ADDRESS) ;
		char temp[100] ;
		sprintf(temp, "***Try to boot at sector: %d\n", bootsector) ;
		uart_sendString(temp) ;

		if( 0 <= bootsector && 255 >= bootsector)
		{
			uint32_t Top_SP ;
			uint32_t reset_add ;

			// Take main stack pointer reload value
			Top_SP = Flash_readWord(bootsector * 1024) ;
			// Take address of reset handler
			reset_add =  Flash_readWord(bootsector * 1024 + 4) ;

			sprintf(temp, "***Reset handler: 0x%x ... TopSP: 0x%x \n",  reset_add, Top_SP) ;
			uart_sendString(temp) ;

			/* Add some delay*/
			int j = 100000 ;
			while(j --) ;

			// Check if the LSB bit of address if 1 (Thumb instruction set)
			if((reset_add & 1) && (reset_add <= (uint32_t)&__top_Flash ) && (Top_SP >= (uint32_t)&__base_RAM) && (Top_SP <= (uint32_t)&__top_RAM ))
			{

				system_denit() ;
				// Set new MSP
				__disable_irq() ;
				__set_MSP(Top_SP) ;
				__enable_irq() ;
				// Change VTOR
				SCB->VTOR = (uint32_t)(bootsector * 1024) ;
				// jump to reset handler
				((void (*) (void)) (reset_add)) () ;
			}
		}
		uart_sendString("Fail to boot\n") ;
		while(1) ;
	}
}

/************************ End of Main function *************************/

void system_init()
{
	/* Relocate vector table to RAM*/
	for(int i = 0; i < 47 ; i ++)
		{
			copy[i] = g_pfnVectors[i] ;
		}
	SCB->VTOR = (uint32_t)copy ;

	/* Init GPIO pin*/
	// Button pin
	PORT_PinConfigType Button_PORTconfig = {
			.ALTMode = PORT_ALT_GPIO ,
			.pull = PORT_PULLUP,
			.speed = PORT_SPEED_LOW ,
			.int_config = PORT_INTERRUPT_DISABLE,
	};
	PORT_InitPin(PORTC, PORT_PIN_3, &Button_PORTconfig) ;

	GPIO_PinConfigType Button_GPIOconfig = {
			.direction = GPIO_INPUT,
	};
	GPIO_InitPin(GPIOC, GPIO_PIN_3, &Button_GPIOconfig) ;
	// UART pin
	PORT_PinConfigType uart_port_config = {
			.ALTMode = PORT_AFT_2,
			.pull = PORT_PULLUP,
	};
	PORT_InitPin(PORTA, 1, &uart_port_config) ;
	PORT_InitPin(PORTA, 2, &uart_port_config) ;

	/* Initial UART*/
	uart_init() ;
}

void system_denit()
{
	GPIO_DenitPin(GPIOC, GPIO_PIN_3) ;
	PORT_DenitPin(PORTC, PORT_PIN_3) ;
	PORT_DenitPin(PORTA, 1) ;
	PORT_DenitPin(PORTA, 2) ;
	SIM->SCGC5 &= ~SIM_SCGC5_PORTC_MASK ;
	SIM->SCGC5 &= ~SIM_SCGC5_PORTA_MASK ;

	uart_denit() ;
}

void LOG(BootloaderStatus_t status, char* str ){
	char temp[10];

	if(status != BLD_OK && status != BLD_MESSAGE)
	{
		sprintf(temp, "[%d] ", status) ;
		uart_sendString(temp) ;
	}

	switch(status)
	{
	case BLD_OK :
		break;
	case BLD_READDY_TO_FLASH:
		uart_sendString("Ready to Flash\n") ;
		break;
	case BLD_ERR_ERASED:
		uart_sendString("Erase sector error\n") ;
		break ;
	case BLD_FLASH_ERR:
		uart_sendString("Error while flashing word\n") ;
		break;
	case BLD_ERR_DATA:
		uart_sendString("Wrong data line\n") ;
		break;
	case BLD_FLASH_FINISH:
		uart_sendString("Flash finished\n") ;
		break;
	case BLD_SYNTAX_ERR:
		uart_sendString("Syntax error\n") ;
		break;
	case BLD_HAVE_NOT_ERASE_YET:
		uart_sendString("Sectors have not erased yet\n") ;
		break;
	case BLD_ERR_CREATBOOT:
		uart_sendString("Wrong data line\n") ;
		break;
	case BLD_BOOT_FAIL:
		uart_sendString("Boot to sector fail\n") ;
		break;
	case BLD_MESSAGE:
		break;
	}
	if(str != NULL)
	{
		uart_sendString("***") ;
		uart_sendString(str) ;
	}

}


