#include "MKL46Z4.h"
#include "core_cm0plus.h"
#include "cmsis_gcc.h"
#include "uart.h"
#include "port.h"
#include "gpio.h"
#include "flash.h"

#include "string.h"

#define BOOT_TABLE_ADDRESS (0x3FC00)		// Last sector (255)
#define BOOT_TABLE_SIGN (0xABCD)


typedef enum{
	BLD_OK = 0u,
	BLD_ERR = 1u,
	BLD_READDY_TO_FLASH,
	BLD_FLASH_ERR,
	BLD_ERR_DATA,
	BLD_FLASH_FINISH,
	BLD_SYNTAX_ERR,
	BLD_HAVE_NOT_ERASE_YET,
	BLD_ERR_CREATBOOT,
	BLD_MESSAGE,
	BLD_BOOT_FAIL,
}BootloaderStatus_t;


typedef enum {
	LOAD_NEW_FIRMWARE	= 0u,
	ERASE_SECTOR 		= 1u,
	RESTORE_BACKUP		= 2u,
	RECEIVE_DATA		= 3u
}BootLoaderModeType;

typedef struct{
	uint16_t firstsector ;		// Start sector of this partition
	uint8_t bootable ;			// There any firmware on this partition
}__attribute__ ((aligned (4))) PartitionType;

typedef struct{
	uint32_t boottable_sign ; 	// specified table
	uint8_t current_boot;		// current partition to boot
	PartitionType partition[2];	// list of partitions
}__attribute__ ((aligned (4))) BootTableType;


void Init_System() ;
void Denit_System() ;
BootloaderStatus_t CreatBootTable(BootTableType* boot_table) ;
BootloaderStatus_t BootToSector(uint32_t sector_num) ;
BootloaderStatus_t hex_to_num(uint32_t* des, uint8_t* str, uint8_t num_of_bytes) ;

