#include "BSP_LED.h"

// 关于灯的函数
void BSP_LED_Control(LED_t led, bool flag)
{
  GPIO_PinState state = flag ? GPIO_PIN_RESET : GPIO_PIN_SET;
  switch (led)
  {
  case LED1:
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, state);
    break;
  case LED2:
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, state);
    break;
  case LED3:
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, state);
    break;
  case LED4:
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, state);
    break;
  default:
    break;
  }
}

void BSP_LED_TogglePin(LED_t led)
{
  switch (led)
  {
  case LED1:
    HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_8);
    break;
  case LED2:
    HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
    break;
  case LED3:
    HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10);
    break;
  case LED4:
    HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_11);
    break;
  default:
    break;
  }
}

void BSP_led_on(LED_t led)
{
  BSP_LED_Control(led, true);
}

void BSP_led_off(LED_t led)
{
  BSP_LED_Control(led, false);
}

void LED_Blink(LED_t led, uint8_t cnt, uint16_t delay)
{
  if (led == ALL)
  {
    LED_t leds[] = {LED1, LED2, LED3, LED4};
    int leds_num = sizeof(leds) / sizeof(leds[0]);
    do
    {
      for (int i = 0; i < leds_num; i++)
      {
        BSP_led_on(leds[i]);
      }
      HAL_Delay(delay);
      for (int i = 0; i < leds_num; i++)
      {
        BSP_led_off(leds[i]);
      }
      HAL_Delay(delay);
    } while (cnt--);
  }
  else
  {
    for (int i = 0; i < cnt; i++)
    {
      BSP_led_on(led);
      HAL_Delay(delay);
      BSP_led_off(led);
      HAL_Delay(delay);
    }
  }
}

// void LED_Blink_V2(int led,uint8_t cnt,uint16_t delay)
// {

// }