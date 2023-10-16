#include <flash.h>


FLASH_Status_t FLASH_eraseSector(uint32_t sector_num, uint32_t num_of_sectors)
{
    // Tính toán địa chỉ bắt đầu của sector cần xóa
    uint32_t start_address = sector_num * FLASH_SECTOR_SIZE; // Thay SECTOR_SIZE bằng kích thước của mỗi sector

    // Kiểm tra xem các sector cần xóa có hợp lệ không
    if ((start_address + (num_of_sectors * FLASH_SECTOR_SIZE)) > FLASH_SIZE)
    {
        return FLASH_ERR; // Đã vượt quá kích thước bộ nhớ FLASH
    }

    // Thực hiện xóa từng sector
    for (uint32_t i = 0; i < num_of_sectors; i++)
    {
        // Kiểm tra xóa sector tại địa chỉ start_address
        FLASH_Status_t erase_status = FLASH_checkErase(start_address, 1);

        if (erase_status == FLASH_OK)
        {
            // Nếu xóa thành công, cập nhật start_address cho sector tiếp theo
            start_address += FLASH_SECTOR_SIZE;
        }
        else
        {
            // Nếu xóa không thành công, trả về FLASH_ERR
            return FLASH_ERR;
        }
    }

    return FLASH_OK; // Xóa thành công
}


FLASH_Status_t FLASH_writeWord(uint32_t Data, uint32_t Address) {
    // Kiểm tra xem địa chỉ nằm trong khoảng bộ nhớ FLASH hợp lệ không
    if (Address < FLASH_START_ADDRESS || Address >= FLASH_END_ADDRESS)
    {
        return FLASH_ERR; // Địa chỉ không hợp lệ
    }

    // Kiểm tra xem có cần thiết xóa sector trước khi ghi không
    // (tuỳ theo yêu cầu của phần cứng)
    FLASH_Status_t eraseStatus = FLASH_checkErase(Address, sizeof(uint32_t));
    if (eraseStatus != FLASH_OK) {
        return FLASH_ERR; // Lỗi xóa sector
    }

    // Tiến hành ghi dữ liệu vào địa chỉ FLASH
    /* Wait Previous Command Complete? */
            while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);

            /* Check Error from Previous Command */
            if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) == FTFA_FSTAT_ACCERR_MASK)
                || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) == FTFA_FSTAT_FPVIOL_MASK) )
            {
                FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR;
            }

            /* Write Command and Parameter to FCCOB Registers */
            FTFA->FCCOB0 = CMD_PROGRAM_LONGWORD;
            FTFA->FCCOB1 = (uint8_t)((Address) >> 16);
            FTFA->FCCOB2 = (uint8_t)((Address) >> 8);
            FTFA->FCCOB3 = (uint8_t)(Address);

            FTFA->FCCOB4 = (uint8_t)Data;
            FTFA->FCCOB5 = (uint8_t)(Data >> 8);
            FTFA->FCCOB6 = (uint8_t)(Data >> 16);
            FTFA->FCCOB7 = (uint8_t)(Data >> 24);

    uint32_t *flashPtr = (uint32_t *)Address;
    *flashPtr = Data;

    // Kiểm tra xem dữ liệu đã được ghi đúng không
    if (*flashPtr != Data) {
        return FLASH_ERR; // Lỗi khi ghi dữ liệu
    }

    return FLASH_OK; // Ghi thành công
}

uint32_t FLASH_readWord(uint32_t address)
{
	return *((uint32_t*) address) ;
}

void FLASH_read(uint8_t* des, uint32_t address, uint16_t len) {
    uint8_t *flashPtr = (uint8_t *)address;

    for (uint16_t i = 0; i < len; i++) {
        des[i] = flashPtr[i];
    }
}

FLASH_Status_t FLASH_checkErase(uint8_t sector_num, uint8_t num_of_sectors)
{
    // Wait for the previous command to complete
    while (!(FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK));

    // Clear any previous error flags
    if ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) || (FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK))
    {
        FTFA->FSTAT = FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_FPVIOL_MASK;
        return FLASH_ERR; // Return error if there was a previous error
    }

    // Write the erase sector command and sector number to the FCCOB registers
    FTFA->FCCOB0 = CMD_ERASE_SECTOR;
    FTFA->FCCOB1 = sector_num;
    FTFA->FCCOB2 = 0;
    FTFA->FCCOB3 = 0;

    // Clear CCIF to launch the erase command
    FTFA->FSTAT = FTFA_FSTAT_CCIF_MASK;

    // Wait for the command to complete
    while (!(FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK));

    // Check for errors after the erase command
    if ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) || (FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK))
    {
        FTFA->FSTAT = FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_FPVIOL_MASK;
        return FLASH_ERR; // Return error if there was an error during erase
    }

    // Check if the erase command was successful
    if (FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK)
    {
        return FLASH_ERR; // Flash protection violation error
    }
    else
    {
        return FLASH_OK; // No error, sector is blank (erased)
    }
}




