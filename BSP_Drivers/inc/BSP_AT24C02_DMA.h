
#ifndef __BSP_AT24C02_H
#define __BSP_AT24C02_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include "stm32f1xx_hal.h"

//设备状态
typedef enum
{
    EEPROM_IDLE = 0, //空闲 未被初始化
    EEPROM_READY,  
    EEPROM_BUSY,
    EEPROM_SUCCESS,
    EEPROM_ERROR
}Status_t;

#define NONE_OPERATOR  0 
#define READ_OPERATOR  1
#define WRITE_OPERATOR 2

typedef void(*user_callback_t)(bool); //函数指针类型

//表示从设备
typedef struct 
{
    I2C_HandleTypeDef* hi2c; //I2C句柄指针
    uint16_t dev_address;    //从设备地址
    uint8_t page_size;       //页大小
    bool is_Init;            //是否被初始化

    //添加中断标志位 其实就是全局变量
    volatile Status_t status;
    uint8_t* buffer;
    uint8_t  len;
    uint8_t  operator_type; 
    user_callback_t user_callback;
}eeprom_t;

#define DEV_ADDRESS      0x50
#define PAGE_SIZE        8
#define SCAN_RETYIES     2
#define SCAN_TIME_MS     5
#define POLL_RETYIES     10
#define POLL_TIME_MS     2
#define DEV_TOTAL_SIZE   256
#define READ_TIMEOUT_MS  50
#define WRITE_TIMEOUT_MS 50

bool BSP_AT24C02_Init(I2C_HandleTypeDef* hi2c);
bool BSP_AT24C02_Read_DMA(uint16_t start_address, void *data_out, uint8_t data_len, user_callback_t cb);
bool BSP_AT24C02_Write_Page_DMA(uint16_t start_address,void *data_in, uint8_t data_len, user_callback_t cb);

#ifdef __cplusplus
}
#endif

#endif