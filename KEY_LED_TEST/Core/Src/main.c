/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum
{
  LED1 = 1,
  LED2,
  LED3,
  LED4
} LED_t;

typedef enum
{
  KEY1 = 1,
  KEY2,
  KEY3,
  KEY4
} KEY_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// bool led_state = false;
// const uint32_t KEY_DEBOUNCE_MS = 20;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void BSP_LED_Control(LED_t led, bool flag);
void BSP_led_on(LED_t led);
void BSP_led_off(LED_t led);
bool BSP_KEY_IsPressd(KEY_t key);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  // BSP_led_on(LED1);
  // HAL_Delay(1000);
  // BSP_led_on(LED2);
  // HAL_Delay(1000);
  // BSP_led_on(LED3);
  // HAL_Delay(1000);
  // BSP_led_on(LED4);
  // HAL_Delay(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    BSP_KEY_IsPressd(KEY1) ? BSP_led_on(LED1) : BSP_led_off(LED1);
    BSP_KEY_IsPressd(KEY2) ? BSP_led_on(LED2) : BSP_led_off(LED2);
    BSP_KEY_IsPressd(KEY3) ? BSP_led_on(LED3) : BSP_led_off(LED3);
    BSP_KEY_IsPressd(KEY4) ? BSP_led_on(LED4) : BSP_led_off(LED4);

    // if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_10) == GPIO_PIN_RESET){
    //   HAL_Delay(KEY_DEBOUNCE_MS);
    //   if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_10) == GPIO_PIN_RESET){
    //     led_state = !led_state;
    //     HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,(led_state == true? GPIO_PIN_RESET:GPIO_PIN_SET));
    //   }
    //   while(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_10) == GPIO_PIN_RESET);
    // }
    // if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_10) == GPIO_PIN_RESET){
    //   led_state = !led_state;
    //   HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,(led_state == true? GPIO_PIN_RESET:GPIO_PIN_SET));
    //   // if(led_state == false){
    //   //   HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_RESET);
    //   // }else{
    //   //   HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_SET);
    //   // }
    // }

    // if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_10)==GPIO_PIN_RESET)
    // {
    //   HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_RESET);
    // }else{
    //   HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_SET);
    // }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11, GPIO_PIN_SET);

  /*Configure GPIO pins : PF8 PF9 PF10 PF11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PG6 PG7 PG8 PG10 */
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// 关于按键的函数

bool BSP_KEY_IsPressd(KEY_t key)
{
  bool ret = false;
  switch (key)
  {
  case KEY1:
  {
    if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_10) == GPIO_PIN_RESET)
    {
      HAL_Delay(20);
      if(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_10) == GPIO_PIN_RESET)
      ret = true;
    }
    break;
  }
  case KEY2:
  {
    if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6) == GPIO_PIN_RESET)
    {
      HAL_Delay(20);
      if(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6) == GPIO_PIN_RESET)
      ret = true;
    }
    break;
  }
  case KEY3:
  {
    if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_8) == GPIO_PIN_RESET)
    {
      HAL_Delay(20);
      if(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_8) == GPIO_PIN_RESET)
      ret = true;
    }
    break;
  }
  case KEY4:
  {
    if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7) == GPIO_PIN_RESET)
    {
      HAL_Delay(20);
      if(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7) == GPIO_PIN_RESET)
      ret = true;
    }
    break;
  }
  default:
    break;
  }
  return ret;
}

// bool BSP_KEY_IsPressd(KEY_t key)
// {
//   bool ret = false;
//   switch (key)
//   {
//   case KEY1:
//     if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_10) == GPIO_PIN_RESET)
//     {
//       ret = true;
//     }
//     break;

//   case KEY2:
//     if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6) == GPIO_PIN_RESET)
//     {
//       ret = true;
//     }
//     break;

//   case KEY3:
//     if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_8) == GPIO_PIN_RESET)
//     {
//       ret = true;
//     }
//     break;

//   case KEY4:
//     if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7) == GPIO_PIN_RESET)
//     {
//       ret = true;
//     }
//     break;
//   default:
//     break;
//   }
//   return ret;
// }

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

void BSP_led_on(LED_t led)
{
  BSP_LED_Control(led, true);
}

void BSP_led_off(LED_t led)
{
  BSP_LED_Control(led, false);
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
