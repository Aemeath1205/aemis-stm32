#include "BSP_WS2812B.h"
#include <stdbool.h>

static volatile bool g_is_DMA_Done = false;

static TIM_HandleTypeDef* ghtim = NULL;

static pulse_group_t pulse_group = {0};
static rgb_led_group_t  rgb_led_group = {0};

static void Convert_RGB2Pulse(pulse_group_t* pulse_group_dst, rgb_led_group_t* rgb_group_src)
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            rgb_t rgb = rgb_group_src->leds[i][j];
            pulse_t* ppulse = &(pulse_group_dst->leds[i][j]);

            for(int pos = 0; pos < 8; pos++)
            {
                //G
                if((rgb.g & ( 1 << (7-pos) )) != 0)
                {
                    ppulse->green_ccr_pulse[pos] = BIT1_PULSE;
                }
                else
                {
                    ppulse->green_ccr_pulse[pos] = BIT0_PULSE;                 
                }

                //R
                if((rgb.r & ( 1 << (7-pos) )) != 0)
                {
                    ppulse->red_ccr_pulse[pos] = BIT1_PULSE;
                }
                else
                {
                    ppulse->red_ccr_pulse[pos] = BIT0_PULSE;
                }

                //B
                if((rgb.b & ( 1 << (7-pos) )) != 0)
                {
                    ppulse->blue_ccr_pulse[pos] = BIT1_PULSE;
                }
                else
                {
                    ppulse->blue_ccr_pulse[pos] = BIT0_PULSE;
                }

            }
        }
    }

    for(int i = 0; i < RESET_COUNT; i++)
    {
        pulse_group_dst->reset[i] = RESET_PULSE;
    }
}

void BSP_ConvertRGB(void)
{
    Convert_RGB2Pulse(&pulse_group,&rgb_led_group);
}

//设置一个灯的颜色
void BSP_WS2812BSetColor(uint8_t x, uint8_t y, rgb_t rgb)
{
    if(x >= ROWS || y >= COLS)
        return;
    rgb_led_group.leds[x][y].r = rgb.r;
    rgb_led_group.leds[x][y].g = rgb.g;
    rgb_led_group.leds[x][y].b = rgb.b;
}

//重置所有灯的颜色
void BSP_WS2812BResetColor(void)
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            rgb_led_group.leds[i][j].r = 0;
            rgb_led_group.leds[i][j].g = 0;
            rgb_led_group.leds[i][j].b = 0;
        }
    }
}

void BSP_WS2812BInit(TIM_HandleTypeDef* htim)
{
    if(!htim) return;
    ghtim = htim;
}

bool BSP_WS2812BRefresh(void)
{
    if(ghtim == NULL) return false;

    HAL_StatusTypeDef state = HAL_TIM_PWM_Start_DMA(ghtim, TIM_CHANNEL_3, 
        (uint32_t*)&pulse_group, sizeof(pulse_group)/sizeof(uint16_t));
    
        if(HAL_OK != state) return false;
    else
    {
        g_is_DMA_Done = false;
    }
    return true;
}

void WaitDMADone(void)
{
    while(!g_is_DMA_Done);
}

void BSP_DMADone(void)
{
    g_is_DMA_Done = true;
}