#include "MKL46Z4.h"
#include "core_cm0plus.h"
#include "cmsis_gcc.h"
#include "uart.h"
#include "port.h"
#include "gpio.h"
#include "flash.h"


typedef enum {
	BOOT_OK = 0u ,
	BOOT_FAIL  = 1u
}BOOT_STATUS;

BOOT_STATUS BootToSector(uint32_t sector_num) ;

static inline uint8_t hexdigit_to_num(uint8_t digit) ;
static inline uint8_t hex_to_byte(uint8_t* des, uint8_t* str) ;
uint8_t hex_to_num(uint32_t* des, uint8_t* str, uint8_t num_of_byte) ;
