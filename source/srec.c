

#include "srec.h"

static inline uint8_t digit_to_hex(uint8_t digit){
	if(digit >= '0' && digit <= '9'){
		return (digit - '0') ;
	}
	else if(digit >= 'A' && digit <= 'F'){
		return (digit - 'A' + 10) ;
	}
	else return -1 ;
}

uint8_t hex_to_byte(uint8_t* des, uint8_t* str){
    int8_t temp = 0;
    uint8_t status = 0;
    temp = digit_to_hex(str[1]) ;
    if(temp != -1)
    {
        *des = temp ;
        temp = digit_to_hex(str[0]) ;
        if(temp != -1)
        {
        	*des |= temp << 4 ;
        }
        else status = 1 ;
    }
    else status = 1 ;

    return status ;
}



SREC_Status_t SREC_parse(SREC_t* srec, uint8_t* line ){
	SREC_Status_t status = SREC_OK ;
	uint8_t address_len ;
	uint8_t sum = 0 ;
	uint8_t checksum ;
	uint8_t byte ;

	if('S' == line[0]){
		srec->type = digit_to_hex(line[1]) ;
		if(-1 != srec->type)
		{
			switch(srec->type)
		{
				case S0:
				case S1:
				case S5:
				case S9:
					address_len = 2 ;
					break;
				case S2:
				case S6:
				case S8:
					address_len = 3 ;
					break;
				case S3:
				case S7:
					address_len = 4 ;
					break;
		}
			/* Read byte count*/
			status = hex_to_byte(&(srec->byte_count), &line[2]) ;
			if(0 == status)
			{
				sum += srec->byte_count ;
				/*Read address*/
				srec->address = 0 ;
				for(int i = 0 ; i < address_len; i ++){
					status = hex_to_byte(&byte, &line[4 + 2*i]) ;
					if(0 == status)
					{
						sum += byte ;
//						srec->address |= ((uint32_t)byte) << ( 8 * i) ;
						*((uint8_t*)&(srec->address) + address_len - i - 1) = byte ;
					}
					else break ;
				}

				if( 0 == status)
				{
					//Read data
					for(int i = 0; i < srec->byte_count - address_len - 1; i ++)
					{
						status = hex_to_byte(&byte, &line[4 + 2 * address_len + 2 * i]);
						if(0 == status)
						{
							*(srec->data + i) = byte ;
							sum += byte ;
						}
						else break ;
					}
					if(0 == status){
						//Read checksum
						status = hex_to_byte(&checksum, &line[2 + 2 * (srec->byte_count)]) ;

						if(0 == status){
							if(checksum + sum == 0xff){
								status = SREC_OK ;
								srec->byte_count -= (address_len + 1) ;
							}
							else status = SREC_ERR_CHECKSUM ;
						}
					}
				}
			}
			else status = SREC_ERR_BYTECOUNT ;
		}
		else status = SREC_ERR_TYPE ;
	}
	else status = SREC_ERR_FORMAT ;

	return status ;

}
