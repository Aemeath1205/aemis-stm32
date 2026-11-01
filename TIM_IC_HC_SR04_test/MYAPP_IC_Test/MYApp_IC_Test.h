#ifndef __MYAPP_IC_TEST
#define __MYAPP_IC_TEST

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include "MYBSP_Key.h"
#include "BSP_Usart_Redir.h"
#include "BSP_LED.h"

#define SPEED 0.034f
#define LIMITUP   400.0f
#define LIMITDOWN 2.0f

typedef struct App_handle
{
    TIM_HandleTypeDef* htim_trigger; //10μms     tim7
    TIM_HandleTypeDef* htim_key;     //扫描key的  tim6
    TIM_HandleTypeDef* htim_ic;      //收数据的   tim4
    UART_HandleTypeDef*huart;  
    bool Is_Init;
}App_handle_t;

//配置信息
typedef struct App_configure
{
    bool is_trigger_enable;
    bool is_usart_enable;
    bool is_warningled_enable;
}App_configure_t;

typedef struct App_data
{
    uint32_t rising_time;
    uint32_t failing_time;
    bool is_measure_done;
    float final_distance;
}App_data_t;
 

void App_LoopOnce(void);
void App_SetUp(
    TIM_HandleTypeDef* htim_trigger, 
    TIM_HandleTypeDef* htim_key,
    TIM_HandleTypeDef* htim_ic,
    UART_HandleTypeDef* huart);

#ifdef __cplusplus
}
#endif

#endif