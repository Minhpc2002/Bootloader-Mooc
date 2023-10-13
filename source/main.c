#include "main.h"


static inline uint8_t hexdigit_to_num(uint8_t digit) ;
static inline uint8_t hex_to_byte(uint8_t* des, uint8_t* str) ;

void main(){
	Init_System() ;

	// Read boot table
	BootTableType BootTable ;
	FLASH_read((uint8_t*)&BootTable, BOOT_TABLE_ADDRESS, sizeof(BootTable)) ;

	// Check if boottable is available
	// Else creat new boottable
	if(BOOT_TABLE_SIGN != BootTable.boottable_sign){
		BootTable.boottable_sign = BOOT_TABLE_SIGN ;
		BootTable.current_boot = 0 ;
		BootTable.partition[0].firstsector = 10 ;
		BootTable.partition[0].bootable = 0 ;
		BootTable.partition[0].firstsector = 100 ;
		BootTable.partition[0].bootable = 0 ;
		CreatBootTable(&BootTable) ;
	}

	// Check boot pin level
	if(GPIO_PIN_RESET == GPIO_ReadPin(GPIOC, GPIO_PIN_3)){
	// Bootloader mode
		LOG(BLD_MESSAGE, 	"Entered bootloader\n \
							Bootloader V1.0\n" \
							) ;

		uint8_t buffer[100] ;
		uint8_t ready_to_flash = 0 ;
		// Wait for data to arrive
		while(1){
			// Read a line from uart buffer
			UART_getBytesUtil(buffer, '\n', 100) ;
			BootloaderStatus_t status = BLD_OK ;
			switch(buffer[0]){
				case 'L':
					// take sector number
					uint8_t start_sector ;
					uint8_t num_of_sectors ;

					status = hex_to_num(&start_sector, buffer + 1, 1) ;
					if(BLD_OK == status)
					{
						status = hex_to_num(&num_of_sectors, buffer + 3, 1) ;
						if(BLD_OK == status)
						{
							if(FLASH_OK == FLASH_checkErase(start_sector, num_of_sectors))
							{
								// Ready to flash
								status = BLD_READDY_TO_FLASH ;
								ready_to_flash = 1 ;
							}
							else{
								status = BLD_HAVE_NOT_ERASE_YET ;
								ready_to_flash = 0 ;
							}
						}
					}
					break ;
				case 'R':
					uint8_t partition_num ;

					status = hex_to_num(&partition_num, buffer + 1, 1) ;
					if(BLD_OK == status)
					{
						BootTable.current_boot = partition_num ;
						status = CreatBootTable(&BootTable) ;
						LOG(BLD_MESSAGE, "Reversed to previous firmware") ;
					}
					break ;
				case 'E':
					uint8_t start_sector ;
					uint8_t num_of_sectors ;

					// take sector number...
					status = hex_to_num(&start_sector, buffer + 1, 1) ;
					if(BLD_OK == status)
					{
						//Take num of sector to erase
						status = hex_to_num(&num_of_sectors, buffer + 3, 1) ;

						if(BLD_OK == status)
						{
							status = FLASH_eraseSector(start_sector, num_of_sectors ) ;
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
								for(int i = 0; i < data.count; i ++){
									//Write to flash memory
									if(FLASH_OK != FLASH_writeWord(data.data[i], data.address + 4 * i )){
										ready_to_flash = 0 ;
										status = BLD_FLASH_ERR ;
										break;
									}
								}
							}
							else if(S9 == data.type)
							{
								uint8_t new_partition ;
								// Find where to save new firmware
								if(1 == BootTable.current_boot){
									new_partition = 0 ;
								}
								else{
									new_partition = 1 ;
								}
								// Update boot table
								BootTable.partition[new_partition].firstsector = data.address ;
								BootTable.current_boot = new_partition ;
								status = CreatBootTable(&BootTable) ;
								if(BLD_OK == status){
									//LOG(BLD_MESSAGE, "Flashed new firmware at: ") ;
									status = BLD_FLASH_FINISH ;
								}
							}
						}
						else{
							ready_to_flash = 0 ;
							LOG(BLD_ERR_DATA) ;
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
			LOG(status) ;
		}
	}
	else{
		// Jump to Application code
		PartitionType boot_partition ;
		BootloaderStatus_t status ;
		boot_partition = BootTable.partition[BootTable.current_boot] ;
		LOG(BLD_MESSAGE, "Try to boot at sector: ");
		char temp[20] ;
		sprintf(temp, "0x%x\n", boot_partition.firstsector) ;
		LOG(BLD_MESSAGE, temp);

		if(1 == boot_partition.bootable)
		{
			UART_Denit() ;
			status = BootToSector(boot_partition.firstsector);
		}
		else
		{
			LOG(BLD_MESSAGE, "There no firmware...") ;
			status = BLD_BOOT_FAIL ;
		}
		LOG(status) ;
	}
}


BootloaderStatus_t BootToSector(uint32_t sector_num){
	uint32_t Top_SP ;
	uint32_t reset_add ;
	BootloaderStatus_t status = BLD_OK;
	// Take main stack pointer reload value
		Top_SP = FLASH_readWord(sector_num * 1024) ;

	// Take address of reset handler
		reset_add =  FLASH_readWord(sector_num * 1024 + 4) ;

	// Check if the LSB bit of address if 1 (Thumb instruction set)
	if(reset_add & 1)
	{
		// Change VTOR
			SCB->VTOR = (sector_num * 1024) << 7 ;
		// Set new MSP
			__set_MSP(Top_SP) ;
//			__set_PSP(Top_SP) ;
		// jump to reset handler
		((void (*) (void)) reset_add) () ;
	}
	else{
		status = BLD_BOOT_FAIL ;	// This is not address
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

BootloaderStatus_t hex_to_num(uint32_t* des, uint8_t* str, uint8_t num_of_bytes){
	 uint8_t* temp;
	 uint8_t status = 1;
	 temp = (uint8_t*) des ;
	 for(int i = 0; i < num_of_bytes; i ++){
		 status = hex_to_byte(&temp[i], &str[2*i]) ;
		 if(status == 0) break ;
	 }
	 return status?BLD_OK:BLD_ERR_DATA ;
}


void Init_System(){
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
	// Initial UART
	UART_Init() ;
}

void Denit_System(){
	UART_Denit() ;
}

BootloaderStatus_t CreatBootTable(BootTableType* boot_table){
	FLASH_Status_t status = FLASH_OK ;

	status = FLASH_eraseSector(BOOT_TABLE_ADDRESS / 1024, 1) ;
	if(FLASH_OK == status)
	{
		uint8_t* ptr = &boot_table ;
			for(int i = 0;  i< sizeof(boot_table)/ 4; i ++){
				status = FLASH_writeWord(ptr + 4* i, address) ;
				if(FLASH_OK != status) break ;
			}
	}
	return (FLASH_OK == status)? BLD_OK: BLD_ERR_CREATBOOT ;
}


void LOG(BootloaderStatus_t status, uint8_t* str = NULL){

}
