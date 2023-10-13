
#include "MKL46Z4.h"


typedef enum{
	S0 = 0 ,
	S1 = 1 ,
	S9 = 9
}SRECType_t;

typedef struct{
	SRECType_t type ;
	uint32_t address ;
	uint8_t* data ;
	uint32_t count ;
}SREC_t;

typedef enum{
	SREC_OK ,
	SREC_TYPE_ERR ,
	SREC_BYTECOUNT ,
	SREC_CHECKSUM_ERR,
}SRECStatus_t;

SREC_Status SREC_parse(SREC_t* srec, uint8_t* line ){
	// Check type
	// Check byte count
	// Check checksum
}
