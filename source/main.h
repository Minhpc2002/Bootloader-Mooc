#include "MKL46Z4.h"
#include "core_cm0plus.h"
#include "cmsis_gcc.h"
#include "uart.h"
#include "port.h"
#include "gpio.h"
#include "flash.h"
#include "srec.h"
#include "stdio.h"

#define BOOT_TABLE_ADDRESS (0x3FC00)		// Last sector (255)
#define BOOT_TABLE_SIGN (0xABCD)
#define BOOT_WORD_ADDRESS (0x3FC00)


#define BOTTOM_RAM_ADDRESS (0x1FFFE000)
#define TOP_RAM_ADDRESS (0x20005FFF)

typedef enum{
	BLD_OK = 0u,
	BLD_ERR_DATA = 1u,
	BLD_READDY_TO_FLASH,
	BLD_ERR_ERASED,
	BLD_FLASH_ERR,
	BLD_FLASH_FINISH,
	BLD_SYNTAX_ERR,
	BLD_HAVE_NOT_ERASE_YET,
	BLD_ERR_CREATBOOT,
	BLD_MESSAGE,
	BLD_BOOT_FAIL,
}BootloaderStatus_t;

void Init_System() ;
void Denit_System() ;
BootloaderStatus_t BootToSector(uint32_t sector_num) ;
void LOG(BootloaderStatus_t status, char* str ) ;
