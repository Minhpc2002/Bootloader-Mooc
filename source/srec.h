
#include "MKL46Z4.h"

#define MAX_DATA_LEN 32u

typedef enum{
	S0 = 0u,
	S1 = 1u,
	S2 = 2u,
	S3 = 3u,
//	S4 = 4u,
	S5 = 5u,
	S6 = 6u,
	S7 = 7u,
	S8 = 8u,
	S9 = 9u
}SRECType_t;

typedef struct{
	SRECType_t type ;
	uint8_t byte_count ;
	uint32_t address ;
	uint8_t data[MAX_DATA_LEN] ;
}SREC_t;

typedef enum{
	SREC_OK 			= 0u,
	SREC_ERR_FORMAT,
	SREC_ERR_TYPE ,
	SREC_ERR_BYTECOUNT ,
	SREC_ERR_CHECKSUM,
}SREC_Status_t;

SREC_Status_t SREC_parse(SREC_t* srec, uint8_t* line ) ;

uint8_t hex_to_byte(uint8_t* des, uint8_t* str) ;
