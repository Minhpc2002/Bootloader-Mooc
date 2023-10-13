

typedef enum{
	FLASH_READ_ONE_SECTION = 01u,
	FLASH_PROGRAM_CHECK = 02u,
	//something else
}FLASH_CMD;

typedef enum {
	FLASH_OK,
	FLASH_ERR,
}FLASH_Status_t;
FLASH_Status_t FLASH_eraseSector(uint32_t sector_num, uint32_t num_of_sectors);

FLASH_Status_t FLASH_writeWord(uint32_t data, uint32_t address) ;

uint32_t FLASH_readWord(uint32_t address){
	return *((uint32_t*) address) ;
}

void FLASH_read(uint8_t* des, uint32_t address, uint16_t len) ;


FLASH_Status_t FLASH_checkErase(uint8_t sector_num, uint8_t num_of_sector) ;
