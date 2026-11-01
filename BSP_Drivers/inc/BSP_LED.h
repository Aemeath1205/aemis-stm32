#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#include <stdbool.h>

typedef enum
{
 LED1 = 1,
 LED2,
 LED3,
 LED4,
 ALL
} LED_t;

void BSP_led_on(LED_t led);
void BSP_led_off(LED_t led);
void LED_Blink(LED_t led, uint8_t cnt, uint16_t delay);
// void LED_Blink_V2(LED_t led,uint8_t cnt,uint16_t dalay);
void BSP_LED_TogglePin(LED_t led);


#ifdef __cplusplus
}
#endif


#endif