#include "BSP_AT24C02_DMA.h"

eeprom_t at24c02;


static bool BSP_AT24C02_Ready()
{
    if(at24c02.is_Init == false) return false;
    return HAL_OK == HAL_I2C_IsDeviceReady(at24c02.hi2c, at24c02.dev_address, POLL_RETYIES, POLL_TIME_MS);
}


bool BSP_AT24C02_Init(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == NULL)
        return false;
    at24c02.hi2c = hi2c;
    at24c02.dev_address = DEV_ADDRESS << 1;
    at24c02.page_size = PAGE_SIZE;
    at24c02.is_Init = false;

    at24c02.status = EEPROM_IDLE;
    at24c02.buffer = NULL;
    at24c02.len    = 0;
    at24c02.operator_type = NONE_OPERATOR;
    at24c02.user_callback = NULL;

    if (HAL_OK == HAL_I2C_IsDeviceReady(at24c02.hi2c, at24c02.dev_address, SCAN_RETYIES, SCAN_TIME_MS))
    {
        at24c02.is_Init = true;
        at24c02.status = EEPROM_READY;
    }
    return at24c02.is_Init;
}

// 按字节，按page，按任意度
bool BSP_AT24C02_Read_DMA(uint16_t start_address, void *data_out, uint8_t data_len, user_callback_t cb)
{
    if (at24c02.is_Init == false || data_out == NULL || data_len == 0)
    {
        if(cb != NULL) cb(false);
        return false;
    }
    if ((start_address + data_len) > DEV_TOTAL_SIZE)
    {
        if(cb != NULL) cb(false);
        return false;
    }
    if(at24c02.status == EEPROM_BUSY)
    {
        if(cb) cb(false);
        return false;
    }
    at24c02.status = EEPROM_BUSY;
    at24c02.buffer = data_out;
    at24c02.len    = data_len;
    at24c02.operator_type = READ_OPERATOR;
    at24c02.user_callback = cb;


    HAL_StatusTypeDef state = HAL_I2C_Mem_Read_DMA(
        at24c02.hi2c, at24c02.dev_address,
        start_address, I2C_MEMADD_SIZE_8BIT,
        data_out, data_len);

    if(state != HAL_OK)
    {
        if(cb)
        cb(false);
        return false;
    }
    return true;
}


//页内写入
bool BSP_AT24C02_Write_Page_DMA(uint16_t start_address,void *data_in, uint8_t data_len, user_callback_t cb)
{
    if(at24c02.is_Init == false || data_in == NULL || data_len == 0)  
    {
        if(cb) cb(false);
        return false;
    }
    // uint16_t page_num    = start_address / at24c02.page_size;
    uint16_t page_offset = start_address % at24c02.page_size;

    if( page_offset + data_len > at24c02.page_size ) 
    {
        if(cb) cb(false);
         return false;
    }

    if(at24c02.status == EEPROM_BUSY)
    {
        if(cb) cb(false);
        return false;
    }

    at24c02.status = EEPROM_BUSY;
    at24c02.buffer = data_in;
    at24c02.len    = data_len;
    at24c02.operator_type = WRITE_OPERATOR;
    at24c02.user_callback = cb;
    

    HAL_StatusTypeDef state = HAL_I2C_Mem_Write_DMA(
        at24c02.hi2c, at24c02.dev_address,
        start_address, I2C_MEMADD_SIZE_8BIT,
        data_in, data_len);
    
    if(state != HAL_OK) 
    {
        if(cb) cb(false);
        return false;
    }

    return true;
    // return BSP_AT24C02_Ready();
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(at24c02.hi2c->Instance == hi2c->Instance)
    {
        //只处理都
        if(at24c02.operator_type == READ_OPERATOR)
        {
            at24c02.status = EEPROM_SUCCESS;
            if(at24c02.user_callback)
            {
                at24c02.user_callback(true);
                at24c02.user_callback = NULL;
            }
        }
    }
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(at24c02.hi2c->Instance ==hi2c->Instance)
    {
        // BSP_AT24C02_Ready();
        if(at24c02.operator_type == WRITE_OPERATOR)
        {
            at24c02.status = EEPROM_SUCCESS;
            if(at24c02.user_callback)
            {
                at24c02.user_callback(true);
                at24c02.user_callback = NULL;
            }
        }
    } 
}



// //支持任意长度写入，且自动跨页
// bool BSP_AT24C02_Write(uint16_t start_address, void *data_in, uint8_t data_len)
// {
//     if(at24c02.is_Init == false || data_in == NULL || data_len == 0)  return false;

//     if(start_address + data_len > DEV_TOTAL_SIZE) return false;

//     uint16_t bytes_written = 0;//已经写了多少个字节
//     while(bytes_written < data_len)
//     {
//         uint16_t data_left = data_len - bytes_written;

//         uint16_t new_start_address = start_address + bytes_written;

//         // uint16_t current_page_num  = new_start_address / at24c02.page_size;

//         uint16_t page_offset = new_start_address % at24c02.page_size;

//         uint16_t page_space = at24c02.page_size - page_offset;


//         // if(page_space < data_left)
//         // {
//         //     BSP_AT24C02_Write_Page(new_start_address, data_in , page_space);
//         //     bytes_written += page_space;
//         // }
//         // else
//         // {
//         //     BSP_AT24C02_Write_Page(new_start_address, data_in , page_offset);
//         //     bytes_written = data_len;
//         // }

//         uint16_t write_bytes = data_left;
//         if(write_bytes > page_space)
//         {
//             write_bytes = page_space;
//         }
//         if(!BSP_AT24C02_Write_Page(new_start_address, (uint8_t*)data_in + bytes_written, write_bytes))
//         {
//             return false;
//         }
//         bytes_written += write_bytes;
//     }
// }
