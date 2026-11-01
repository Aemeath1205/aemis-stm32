#ifndef  __INTERNAL_FLASH_H
#define __INTERNAL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include "BSP_Usart_Redir.h"

const uint32_t g_page_size        =   FLASH_PAGE_SIZE;
const uint32_t g_page_offset_mask =   FLASH_PAGE_SIZE - 1;
const uint32_t g_page_base_mask   = ~(FLASH_PAGE_SIZE - 1);

void Internal_Flash_AutoTest(void);
void Internal_Flash_Read(
    uint32_t start_address, 
    uint8_t* out_buffer, 
    uint32_t len);
    
bool Internal_Flash_ErasePages(
    uint32_t start_page_address, 
    uint32_t page_num);
bool Internal_Flash_EraseMase(void);

bool Internal_Flash_Program(
    uint32_t start_address,
    const uint8_t* inbuffer,
    uint32_t len);
//写前会擦除
bool Internal_Flash_Write(
    uint32_t start_address, 
    const uint8_t* inbuffer, 
    uint32_t len);



#ifdef __cplusplus
}
#endif


#endif