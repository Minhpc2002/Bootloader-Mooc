

#include "main.h"
#define BOOT_TABBLE_ADDRESS 0xFF00000		// Last sector (255)

typedef enum {
	LOAD_NEW_FIRMWARE	= 0u,
	ERASE_SECTOR 		= 1u,
	RESTORE_BACKUP		= 2u,
	RECEIVE_DATA		= 3u
}BootLoaderModeType;

typedef struct{
	uint8_t num ;
	uint16_t firstsector ;
	uint8_t bootable ;
}PartitionType;

typedef struct{
	PartitionType main_partition;
	PartitionType backup_partition ;
}BootTableType;

BootLoaderModeType bootloader_mode ;
void main(){
	// Init button 1 pin
	PORT_Type Button_PORTconfig = {
			.ALTMode = PORT_ALT_GPIO ,
			.pull = PORT_PULLUP,
			.speed = PORT_SPEED_LOW ,
			.int_config = PORT_INTERRUPT_DISABLE,
	};
	PORT_InitPin(PORTC, PORT_PIN_3, &Button_PORTconfig) ;

	GPIO_PinConfigType Button_GPIOconfig = {
			.direction = GPIO_INPUT,
	};
	GPIO_InitPin(GPIOC, GPIO_PIN_3, Button_GPIOconfig) ;

	//
	if(GPIO_PIN_RESET == GPIO_ReadPin(GPIOC, GPIO_PIN_3)){
	// Bootloader mode
		// Initial UART
		UART_Init() ;
		// Send debug information
		UART_sendString("Flash Mode\n") ;
		// Wait for data to arrive
		uint8_t buffer[100] ;
		uint32_t sector_num ;
		uint32_t num_of_sector ;
		uint8_t ready_to_flash = 0 ;
		while(1){
			UART_getBytesUtil(buffer, '\n', 100) ;  // Read a line from uart buffer
			switch(buffer[0]){
			case 'L':
				bootloader_mode = LOAD_NEW_FIRMWARE ;
			case 'R':
				bootloader_mode = RESTORE_BACKUP ;
			case 'E':
				bootloader_mode = ERASE_SECTOR ;
				// take sector number...
				uint8_t status = 0 ;
				status = hex_to_num(&sector_num, buffer + 1, 4) ;
					if(status == 0){
						UART_sendString("Error\n") ;
						break ;
					}
				//Take num of sector to erase
				if(bootloader_mode == ERASE_SECTOR){
					status = hex_to_num(&num_of_sector, buffer + 5, 4) ;
					if(status == 0){
						UART_sendString("Error\n") ;
						break ;
					}
				}
				break;
			case 'S':
				bootloader_mode == RECEIVE_DATA ;
				break;
			}

			// Process
			if(RECEIVE_DATA == bootloader_mode){

			}
			else if(LOAD_NEW_FIRMWARE == bootloader_mode){
				//Check erase
				// Ready to flash
				UART_sendString("Ready\n") ;
			}
			else if(RESTORE_BACKUP == bootloader_mode){

			}
			else if(ERASE_SECTOR == bootloader_mode){

			}
		}
	}
	else{
		// Read Bootable
		BootTableType BootTable ;
		FLASH_read((uint8_t*)&BootTable, BOOT_TABBLE_ADDRESS, sizeof(BootTable)) ;
		// Jump to Application code
		if( BOOT_FAIL == BootToSector(BootTable.main_partition.firstsector)){
			while(1) ;
		}
	}
}


BOOT_STATUS BootToSector(uint32_t sector_num){
	uint32_t Top_SP ;
	uint32_t reset_add ;
	BOOT_STATUS status = BOOT_OK;
	// Take main stack pointer reload value
		Top_SP = FLASH_readWord(sector_num * 1024) ;
	// Take address of reset handler
		reset_add =  FLASH_readWord(sector_num * 1024 + 4) ;
	if(reset_add & 1){		// Check if the LSB bit of address if 1 (Thumb instruction set)
		// Change VTOR
			SCB->VTOR = (sector_num * 1024) << 7 ;
		// Set new MSP
			__set_MSP(Top_SP) ;
			__set_PSP(Top_SP) ;
		// jump to reset handler
		((void (*) (void)) reset_add) () ;
	}
	else{
		status = BOOT_FAIL ;	// This is not address
	}

	return status ;
}

static inline uint8_t hexdigit_to_num(uint8_t digit){
	if(digit >= '0' && digit <= '9'){
		return (digit - '0') ;
	}
	else if(digit >= 'A' && digit <= 'F'){
		return (digit - 'A' + 10) ;
	}
	else return -1 ;
}

static inline uint8_t hex_to_byte(uint8_t* des, uint8_t* str){
    int8_t temp = 0;
    uint8_t status = 1;
    temp = hexdigit_to_num(str[1]) ;
    if(temp != -1){
        *des = temp ;
    }
    else status = 0 ;
    temp = hexdigit_to_num(str[0]) ;
    if(temp != -1){
        *des |= temp << 4 ;
    }
    else status = 0 ;
    return status ;
}

uint8_t hex_to_num(uint32_t* des, uint8_t* str, uint8_t num_of_byte){
    // Little endian
    uint8_t temp = 0 ;
    uint8_t status = 1;
    *des = 0 ;
    for(uint8_t i = 0; i < num_of_byte; i++){
        status = hex_to_byte(&temp, &str[2*i]) ;
        if(status == 1){
            *des |= ((uint32_t)temp) << (8*i) ;
        }
        else{
            break ;
        }
    }
    return status ;
}
