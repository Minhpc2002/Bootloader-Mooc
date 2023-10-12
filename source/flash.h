

typedef enum{
	FLASH_READ_ONE_SECTION = 01u,
	FLASH_PROGRAM_CHECK = 02u,
	//something else
}FLASH_CMD;
void FLASH_eraseSector(uint32_t sector_num, uint32_t num_of_sector){

}

void FLASH_writeWord(uint32_t data, uint32_t address){

}

uint32_t FLASH_readWord(uint32_t address){

}
