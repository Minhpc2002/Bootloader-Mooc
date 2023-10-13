#include "MKL46Z4.h"
#include "core_cm0plus.h"
#include "cmsis_gcc.h"
#include "uart.h"
#include "port.h"
#include "gpio.h"
#include "flash.h"

#define BOOT_TABBLE_ADDRESS 0xFF00000		// Last sector (255)


typedef enum {
	BOOT_OK = 0u ,
	BOOT_FAIL  = 1u
}BOOT_STATUS;


typedef enum {
	LOAD_NEW_FIRMWARE	= 0u,
	ERASE_SECTOR 		= 1u,
	RESTORE_BACKUP		= 2u,
	RECEIVE_DATA		= 3u
}BootLoaderModeType;

typedef struct{
	uint16_t firstsector ;
	uint8_t bootable ;
}__attribute__ ((aligned (4))) PartitionType;

typedef struct{
	uint32_t boottable_sign ;
	uint8_t current_boot;
	PartitionType partition[2];
}__attribute__ ((aligned (4)))BootTableType;


void Init_System() ;
void Denit_System() ;
BOOT_STATUS BootToSector(uint32_t sector_num) ;
static inline uint8_t hexdigit_to_num(uint8_t digit) ;
static inline uint8_t hex_to_byte(uint8_t* des, uint8_t* str) ;
uint8_t hex_to_num(uint32_t* des, uint8_t* str, uint8_t num_of_byte) ;
