#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#include <stdbool.h>

typedef enum
{
    KEY_STATE_PRESS = 0,
    KEY_STATE_RELEASE
}key_state;

//按键句柄
typedef struct 
{
    key_state state;       //按键状态
    uint16_t debounce_cnt; //消抖计数器
}key_t;

#define KEY_NUM 4

//分装端口位和pin
#define KEY0_PORT GPIOG
#define KEY0_PIN  GPIO_PIN_10

#define KEY1_PORT GPIOG
#define KEY1_PIN  GPIO_PIN_6

#define KEY2_PORT GPIOG
#define KEY2_PIN  GPIO_PIN_8

#define KEY3_PORT GPIOG
#define KEY3_PIN  GPIO_PIN_7

//定义按键活跃状态
#define KEY_ACTIVE_LEVEL 0

//延时时长
#define DEBOUNCE_MS 20

//设置按键回调
//哪一个，发送了什么
typedef void(*KeyCallBack_t)(uint8_t key_id, key_state state);

void Key_Init(KeyCallBack_t cb);
void Key_Scan(void);


#ifdef __cplusplus
}
#endif


#endif