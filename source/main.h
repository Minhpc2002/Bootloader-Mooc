

#include "MKL46Z4.h"
#include "core_cm0plus.h"
#include "system_MKL46Z4.h"
#include "port.h"
#include "gpio.h"
#include "uart.h"
#include "srec.h"
#include "flash.h"
#include "stdio.h"


#define BOOT_WORD_ADDRESS (0x3800)  // sector 14


#define BOTTOM_RAM_ADDRESS (0x1FFFE000)
#define TOP_RAM_ADDRESS (0x20006000)

typedef enum{
	BLD_OK = 0u,
	BLD_ERR_DATA = 1u,
	BLD_ERR_ERASED,
	BLD_READDY_TO_FLASH,
	BLD_FLASH_ERR,
	BLD_FLASH_FINISH,
	BLD_SYNTAX_ERR,
	BLD_HAVE_NOT_ERASE_YET,
	BLD_ERR_CREATBOOT,
	BLD_MESSAGE,
	BLD_BOOT_FAIL,
}BootloaderStatus_t;

void system_init() ;
void system_denit() ;
void LOG(BootloaderStatus_t status, char* str ) ;
