#ifndef __APP_IC_H
#define __APP_IC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "BSP_LED.h"
#include "BSP_Key.h"
#include "BSP_Usart_Redir.h"
#include <stdbool.h>

#define SPEED 0.034f
#define LIMITUP 400.0f
#define LIMITDOWM 2.0f

typedef struct app_configure
{
    bool is_ultrasonic_ranging_enable;
    bool is_usart_enable;
    bool is_warning_enable;
}app_configure_t;


typedef struct app_handler
{
    TIM_HandleTypeDef* htim_trigger;
    TIM_HandleTypeDef* htim_key;
    TIM_HandleTypeDef* htim_ic;
    UART_HandleTypeDef* huart;
    bool is_init;
}app_handler_t;


void App_SetUp(
    TIM_HandleTypeDef* htim_trigger, 
    TIM_HandleTypeDef* htim_key,
    TIM_HandleTypeDef* htim_ic,
    UART_HandleTypeDef* huart);
void App_LoopOnce();

typedef struct app_measure
{
    uint32_t rising_time;
    uint32_t falling_time;
    bool measure_done;
    float distance_cm;
}app_measure_t;


#ifdef __cplusplus
}
#endif


#endif