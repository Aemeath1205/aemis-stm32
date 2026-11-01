#ifndef __MYBSP_KEY_H
#define __MYBSP_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "BSP_LED.h"
#include "stdbool.h"

#define LEDNUMS     3
#define DEBOUNCECNT 20

typedef enum Key_state
{
    State_Release = 1,
    State_Pressed
}Key_state_t;

typedef enum Key_id
{
    KEY1 = 1,
    KEY2,
    KEY3,
    KEY4
}Key_id_t;

typedef struct Single_key
{
    Key_id_t    key_id;
    Key_state_t key_state;
    bool        is_init;          //是否初始化
    uint8_t     debounce_cnt;
// LED_t        key_to_led;       //按键所对应的灯
}Single_key_t;

#define KEY1_PORT GPIOG
#define KEY1_PIN  GPIO_PIN_10

#define KEY2_PORT GPIOG
#define KEY2_PIN  GPIO_PIN_6

#define KEY3_PORT GPIOG
#define KEY3_PIN  GPIO_PIN_8

#define KEY4_PORT GPIOG
#define KEY4_PIN  GPIO_PIN_7

typedef void(*KeyCallBack_t)(uint8_t key_id, Key_state_t key_state);
void Key_Init(KeyCallBack_t cb);
void Key_Scan(void);//扫描所有按键 



#ifdef __cplusplus
}
#endif

#endif  