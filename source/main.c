#include <main.h>


Flash_Status_t flash_err = FLASH_OK ;
volatile uint32_t count = 0 ;

void main(){
	Init_System() ;

	// Check boot pin level
	if(GPIO_PIN_RESET == GPIO_ReadPin(GPIOC, GPIO_PIN_3)){
	// Bootloader mode

		LOG(BLD_MESSAGE, "Entered bootloader : version: Bootloader V1\n") ;

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
			UART_readUntil(buffer, '\n') ;

			switch(buffer[0]){

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
							}
							else{
								status = BLD_HAVE_NOT_ERASE_YET ;
							}
						}
					}
					break ;
				case 'R':

					status = hex_to_byte(&start_sector, buffer + 1) ;

					if(BLD_OK == status)
					{
						Flash_eraseSector(BOOT_WORD_ADDRESS / 1024) ;

						uint32_t value = start_sector ;
						Flash_writeWord(BOOT_WORD_ADDRESS, &value) ;

						sprintf(temp, "Reversed to previous firmware at sector 0x%x\n", start_sector) ;

						LOG(BLD_MESSAGE, temp) ;
					}
					break ;

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
								sprintf(temp, "Erased at %d sector from 0x%x\n",num_of_sectors,  start_sector) ;
								LOG(BLD_MESSAGE, temp) ;
							}
							else{
								status = BLD_ERR_ERASED ;
							}
						}
					}
					break;
				case 'S':
					if(ready_to_flash == 1)
					{
						SREC_t data;

						if(SREC_OK == SREC_parse(&data, buffer))
						{
							if(S1 == data.type )
							{
								for(int i = 0; i < data.byte_count ; i += 4){
									// Write to flash memory
									flash_err = Flash_writeWord(data.address + i, (uint32_t*)(data.data + i)) ;
									if(FLASH_OK != flash_err)
									{
//										status = BLD_FLASH_ERR ;
//										ready_to_flash = 0 ;
//										break;
									}
									else {
										status = BLD_OK ;
										count ++ ;
									}
								}
							}
							else if(S9 == data.type)
							{
								status = BLD_FLASH_FINISH ;
								ready_to_flash = 0 ;
							}
						}
						else{
							ready_to_flash = 0 ;
							status = BLD_ERR_DATA ;
						}
					}
//					else
//					{
//						status = BLD_HAVE_NOT_ERASE_YET ;
//					}
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
		// Jump to Application code
		uint8_t bootsector = Flash_readWord(BOOT_WORD_ADDRESS) ;

		char temp[100] ;
		sprintf(temp, "Try to boot at sector: %d\n", bootsector) ;

		LOG(BLD_MESSAGE, temp);

		if( BLD_OK != BootToSector(bootsector))
		{
			LOG(BLD_MESSAGE, "Fail to boot\n");
			while(1) ;
		}
	}
}


BootloaderStatus_t BootToSector(uint32_t sector_num){
	uint32_t Top_SP ;
	uint32_t reset_add ;
	BootloaderStatus_t status = BLD_OK;
	// Take main stack pointer reload value
		Top_SP = Flash_readWord(sector_num * 1024) ;

	// Take address of reset handler
		reset_add =  Flash_readWord(sector_num * 1024 + 4) ;

	char temp[100] ;
	sprintf(temp, "\nsector_num: %d reset add: %x TopSP: %x \n",sector_num,  reset_add, Top_SP) ;
	UART_sendString(temp) ;

	// Check if the LSB bit of address if 1 (Thumb instruction set)
	if((reset_add & 1) && (Top_SP >= BOTTOM_RAM_ADDRESS) && (Top_SP <= TOP_RAM_ADDRESS) )
	{
		status = BLD_OK ;
		// Change VTOR
			SCB->VTOR = (sector_num * 1024) << 7 ;
		// Set new MSP
			__set_MSP(Top_SP) ;
//			__set_PSP(Top_SP) ;
		// jump to reset handler
		((void (*) (void)) (reset_add - 1)) () ;
	}
	else{
		status = BLD_BOOT_FAIL ;	// This is not address
	}
	return status ;
}



void Init_System(){
	// Init button 1 pin
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
	// Initial UART
	UART_init() ;
}

void Denit_System(){
	UART_denit() ;
	PORT_DenitPin(PORTC, PORT_PIN_3) ;
}



void LOG(BootloaderStatus_t status, char* str ){
	char temp[10];
	sprintf(temp, " [%d] ", status) ;
	if(status != BLD_OK && status != BLD_MESSAGE)
	{
		UART_sendString(temp) ;
	}

	switch(status)
	{
	case BLD_OK :
		break;
	case BLD_READDY_TO_FLASH:
		UART_sendString(" Ready to Flash\n") ;
		break;
	case BLD_ERR_ERASED:
		UART_sendString(" Erase sector error\n") ;
		break ;
	case BLD_FLASH_ERR:
		UART_sendString(" Error while flashing word\n") ;
		break;
	case BLD_ERR_DATA:
		UART_sendString(" Wrong data line\n") ;
		break;
	case BLD_FLASH_FINISH:
		UART_sendString(" Flash finished\n") ;
		break;
	case BLD_SYNTAX_ERR:
		UART_sendString(" Syntax error\n") ;
		break;
	case BLD_HAVE_NOT_ERASE_YET:
		UART_sendString(" Sectors have not erased yet\n") ;
		break;
	case BLD_ERR_CREATBOOT:
		UART_sendString(" Wrong data line\n") ;
		break;
	case BLD_BOOT_FAIL:
		UART_sendString(" Boot to sector fail\n") ;
		break;
	case BLD_MESSAGE:
		break;
	}
	if(str != NULL)
	{
		UART_sendString(str) ;
	}

}
