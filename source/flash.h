#ifndef _FLASH_H_
#define _FLASH_H_

#include "MKL46Z4.h"

#define FLASH_SECTOR_SIZE 1024U
#define FLASH_SIZE 0x10000  //  kích thước flash là 64KB
#define FLASH_START_ADDRESS 0x00000000
#define FLASH_END_ADDRESS   0x0007FFFF
#define FTFA_FSTAT_CLEAR_ERROR          0x30U
#define START_COMMAND                   0x80U
#define CMD_PROGRAM_LONGWORD            0x06U
#define CMD_ERASE_SECTOR               0x09U

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

uint32_t FLASH_readWord(uint32_t address);

void FLASH_read(uint8_t* des, uint32_t address, uint16_t len) ;


FLASH_Status_t FLASH_checkErase(uint8_t sector_num, uint8_t num_of_sectors) ;

#endif /* _FLASH_H_ */
