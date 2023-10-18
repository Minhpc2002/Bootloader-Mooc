#include "flash.h"

typedef enum {
	FCMD_READ_1S_SECTION 	= 0x01u,
	FCMD_PROGRAM_WORD 		= 0x06u ,
	FCMD_ERASE_SECTOR 		= 0x09u
}FCMD_Type ;




#define FTFA_FSTAT_CLEAR_ERROR_MARK          0x30U
#define START_COMMAND_MARK                   0x80U


Flash_Status_t Flash_writeWord(uint32_t address, uint32_t* data)
{
	Flash_Status_t status = FLASH_OK ;
	uint8_t* ptr = (uint8_t*) data ;

	/* Wait previous command complete*/
		while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);

		/* Check Error from Previous Command */
		if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0 ) || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) != 0 ))
		{
			// Clear error of previous command
			FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR_MARK;
		}

	/* Write Command and Parameter to FCCOB Registers */
	FTFA->FCCOB0 = FCMD_PROGRAM_WORD;
	FTFA->FCCOB1 = (uint8_t)((address) >> 16);
	FTFA->FCCOB2 = (uint8_t)((address) >> 8);
	FTFA->FCCOB3 = (uint8_t)(address);

	FTFA->FCCOB4 = *(ptr+3);
	FTFA->FCCOB5 = *(ptr+2);
	FTFA->FCCOB6 = *(ptr+1);
	FTFA->FCCOB7 = *ptr;

	/* Clear CCIF Flag to launch command */
	FTFA->FSTAT = START_COMMAND_MARK;

	/* Wait previous command complete*/
	while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);

	/* Check Error from Previous Command */
	if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0 ) || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) != 0 ))
	{
		status = FLASH_WRITE_FAIL ;
		// Clear error of previous command
//		FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR_MARK;
	}
	else status = FLASH_OK ;

	return status ;

}

Flash_Status_t Flash_eraseSector(uint8_t sector_num){

	Flash_Status_t status = FLASH_OK ;
	if(sector_num <= 1){
		status =  FLASH_ERR;
	}
	else
	{
	uint32_t address = sector_num * 1024 ;

	/* Wait previous command complete*/
		while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);

		/* Check Error from Previous Command */
		if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0 ) || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) != 0 ))
		{
			// Clear error of previous command
			FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR_MARK;
		}

	/* Write new command*/
		FTFA->FCCOB0 = FCMD_ERASE_SECTOR ;
		FTFA->FCCOB1 = (uint8_t) (address >> 16) ;
		FTFA->FCCOB2 = (uint8_t) (address >> 8) ;
		FTFA->FCCOB3 = (uint8_t) (address ) ;

	//Lauch command
		FTFA->FSTAT = 0x80 ;
		// Wait previous command complete
		while((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0) ;

		/* Check if there are errors from previous command */
		if(((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0) || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) != 0) )
		{

//			FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR_MARK ; // Clear error of previous command
			status =  FLASH_ERASE_FAIL ;
		}
		else{
			status =  FLASH_OK ;
		}
	}
	return status ;
}

Flash_Status_t Flash_checkSectorsErased(uint8_t sector_num, uint8_t num_of_sectors)
{
	Flash_Status_t status = FLASH_OK;
	uint32_t address = sector_num * 1024 ;
	uint16_t num_of_long_words = num_of_sectors * 1024 / 4 ;

	/* Wait previous command complete*/
		while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);

		/* Check Error from Previous Command */
		if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0 ) || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) != 0 ))
		{
			// Clear error of previous command
			FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR_MARK;
		}

	/* Write new command*/
	FTFA->FCCOB0 = FCMD_READ_1S_SECTION ;
	FTFA->FCCOB1 = (uint8_t) (address >> 16) ;
	FTFA->FCCOB2 = (uint8_t) (address >> 8) ;
	FTFA->FCCOB3 = (uint8_t) (address ) ;

	FTFA->FCCOB4 = (uint8_t) (num_of_long_words >> 8 ) ;
	FTFA->FCCOB5 = (uint8_t) (num_of_long_words ) ;
	FTFA->FCCOB6 = 0 ;

	//Lauch command
	FTFA->FSTAT = START_COMMAND_MARK ;
	// Wait previous command complete
	while((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0) ;

	/* Check if there are errors from previous command */
	if(((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0) || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) != 0) ){

//		FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR_MARK ; // Clear error of previous command
		status =  FLASH_ERR ;
	}
	else{
		// Check if flash was erased
		if((FTFA->FSTAT & FTFA_FSTAT_MGSTAT0_MASK) != 0)
		{
			// if MGSTAT0 bit set mean fails to read all 1s
			status =  FLASH_CHECK_ERASE_FAIL ;
		}
	}
	return status ;
}

Flash_Status_t Flash_eraseMultiSectors(uint8_t start_sector, uint8_t num_of_sectors)
{
	Flash_Status_t status = FLASH_OK ;
    for (uint32_t i = start_sector; i < start_sector + num_of_sectors; i++)
    {
    	status = Flash_eraseSector(i) ;
    	if(FLASH_OK != status) break ;
    }
    return status ;
}

