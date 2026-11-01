#ifndef __BSP_WS2812B_H
#define __BSP_WS2812B_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdbool.h>


//0码 和 1码 pwm宽度
#define BIT0_PULSE 21
#define BIT1_PULSE 43
#define RESET_PULSE 0

#define ROWS 3
#define COLS 3

#define RESET_COUNT 100

//用户视角

//描述灯的颜色
typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}rgb_t;

//描述有几个灯
typedef struct
{
    rgb_t leds[ROWS][COLS];
}rgb_led_group_t;

//定时器视角

typedef struct 
{
    uint16_t green_ccr_pulse[8];
    uint16_t red_ccr_pulse[8];
    uint16_t blue_ccr_pulse[8];
}pulse_t;

typedef struct
{
    pulse_t leds[ROWS][COLS];
    uint16_t reset[RESET_COUNT];
}pulse_group_t;

void BSP_ConvertRGB(void);
void BSP_WS2812BSetColor(uint8_t x, uint8_t y, rgb_t rgb);
void BSP_WS2812BResetColor(void);
void BSP_WS2812BInit(TIM_HandleTypeDef* htim);
bool BSP_WS2812BRefresh(void);
void WaitDMADone(void);
void BSP_DMADone(void);


#ifdef __cplusplus
}
#endif


#endif