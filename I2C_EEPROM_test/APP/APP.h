#ifndef __APP_H
#define __APP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "BSP_LED.h"
#include "BSP_Usart_Redir.h"
#include "BSP_AT24C02.h"

#define APP_MAGIC   0x35
#define APP_VERSION 0x01

#define CONFIG_ADDRESS 0x0

#define SAVE_CONFIG(conf_addr, conf_len) BSP_AT24C02_Write(CONFIG_ADDRESS, conf_addr, conf_len)
#define LOAD_CONFIG(conf_addr, conf_len) BSP_AT24C02_Read(CONFIG_ADDRESS, conf_addr,conf_len)

bool BSP_AT24C02_Read(uint16_t start_address,void* data_out,uint8_t data_len);
bool BSP_AT24C02_Write(uint16_t start_address,void* data_in,uint8_t data_len);


//8字节配置结构体
typedef struct _config_t
{
    uint8_t magic;      //魔数：配置有效
    uint8_t version;    //版本
    uint8_t led_id;     //哪个led
    uint8_t repeat_cnt; //闪几次
    uint16_t delay_ms;  //时间间隔
    uint16_t start_time;//开始闪灯时间
}_config_t;

void APP_Init();
void APP_Run();

#ifdef __cplusplus
}
#endif

#endif