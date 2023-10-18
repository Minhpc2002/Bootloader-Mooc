#ifndef _FLASH_H_
#define _FLASH_H_

#include "MKL46Z4.h"

typedef enum{
	FLASH_OK 						= 0u,
	FLASH_ERR 						= 1u,
	FLASH_CHECK_ERASE_FAIL,
	FLASH_ERASE_FAIL,
	FLASH_WRITE_FAIL,
}Flash_Status_t;


Flash_Status_t Flash_writeWord(uint32_t address, uint32_t* data) ;
Flash_Status_t Flash_eraseSector(uint8_t sector_num);
Flash_Status_t Flash_eraseMultiSectors(uint8_t start_sector, uint8_t num_of_sectors) ;
Flash_Status_t Flash_checkSectorsErased(uint8_t sector_num, uint8_t num_of_sectors) ;


// notice wait write process complete before read
#define Flash_readWord(address) (*((uint32_t*)(address)))


#endif /* _FLASH_H_ */
