#include "Inernal_Flash.h"

void Internal_Flash_Read(uint32_t start_address, 
    uint8_t* out_buffer, uint32_t len)
{
    for(int i = 0; i < len; i++)
    {
        out_buffer[i] = *((volatile uint8_t*)(start_address) + i);
    }
}

bool Internal_Flash_ErasePages(uint32_t start_page_address, 
    uint32_t page_num)
{
    if(page_num == 0) return false;
    if(start_page_address & g_page_offset_mask) return false;

    bool result = false;
    FLASH_EraseInitTypeDef erase_init = { 0 };
    erase_init.TypeErase   = FLASH_TYPEERASE_PAGES;
    erase_init.PageAddress = start_page_address;
    erase_init.NbPages     = page_num; 

    uint32_t pages_error   = 0;
    HAL_FLASH_Unlock();
    result = (HAL_FLASHEx_Erase(&erase_init, &pages_error) == HAL_OK);
    HAL_FLASH_Lock();

    return result;
}

bool Internal_Flash_EraseMase(void)
{
    FLASH_EraseInitTypeDef erase_init = {0};
    erase_init.Banks       = FLASH_BANK_1;
    erase_init.NbPages     = 0;
    erase_init.PageAddress = 0;
    erase_init.TypeErase   = FLASH_TYPEERASE_MASSERASE;

    uint32_t page_err = 0;

    HAL_FLASH_Unlock();
    HAL_StatusTypeDef state = HAL_FLASHEx_Erase(&erase_init, &page_err);
    HAL_FLASH_Lock();

    return state == HAL_OK;
}

static inline uint16_t PackHalfWord(const uint8_t* start_address, uint32_t remain)
{
    if(remain == 0) return 0;
    // if(remain == 1) return (uint16_t)start_address[0];
    if(remain == 1)  return (uint16_t)start_address[0] | 0xFF00U;
    return ((start_address[0]) | (start_address[1] << 8)); 
}

bool Internal_Flash_Program(
    uint32_t start_address,
    const uint8_t* inbuffer,
    uint32_t len)
{
    if(NULL == inbuffer) return false;
    if(len == 0) return false;

    uint32_t i = 0;
    HAL_FLASH_Unlock();
    for( ; i < len; i += 2)
    {
        uint16_t data = PackHalfWord(inbuffer + i, len - i);
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, start_address + i, data) != HAL_OK)
        break;
    }
    HAL_FLASH_Lock();

    if(i < len) return false;
    return true;
}

static inline uint32_t AlignDown2KB(uint32_t address)
{
    return (address & g_page_base_mask);
}

static inline uint32_t AlignUp2KB(uint32_t address)
{
    return ((address + g_page_size -1) & g_page_base_mask);
}

bool Internal_Flash_Write(
    uint32_t start_address, 
    const uint8_t* inbuffer, 
    uint32_t len)
{
    if(inbuffer == NULL || len == 0) return false;
    if(start_address & 0x1) return false;
    
    //计算擦除page的个数
    //   起始页地址 左闭
    uint32_t start_address_page = AlignDown2KB(start_address);
    //   结束页地址 右开
    uint32_t end_address_page = AlignUp2KB(start_address + len);

    uint32_t page_num = (end_address_page - start_address) / g_page_size;

    bool result = Internal_Flash_ErasePages(start_address_page, page_num);
    if(result) result = Internal_Flash_Program(start_address, inbuffer, len);
    return result;
}





//////////////////////////For Test///////////////////////////////
static void Read_Test(void)
{

}



void Internal_Flash_AutoTest(void)
{
    Read_Test();
}
