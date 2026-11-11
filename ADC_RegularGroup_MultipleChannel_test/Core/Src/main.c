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
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BSP_Usart_Redir_V2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include <stdbool.h>
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile bool g_dma_complete = false;
volatile bool g_dma_half_complete = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
// static void ADC_Read_MultipleChannel_Block(ADC_HandleTypeDef* hadc, uint32_t* values, uint32_t count);
static float ADC_Code_to_Voltage(uint32_t adc_value);
// static void ADC_Read_MultipleChannel_Test(void);
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
  BSP_Usart_Init(&huart1);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // volatile uint16_t values[4] = { 0 };
  volatile uint16_t values[8] = {0};
  const char *channel_name[] =
      {
          "ADC_CHANNEL_11",
          "ADC_CHANNEL_10"
          "ADC_CHANNEL_5",
          "ADC_CHANNEL_4",
  };
  HAL_ADCEx_Calibration_Start(&hadc3);
  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)values, 4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // DMA循环 ADC连续 Ping-Pong buffer
    if (g_dma_half_complete)
    {
      for (int i = 0; i < 4; i++)
      {
        printf(">>> %s, value : %d, voltage: %.2f\n", channel_name[i], values[i], ADC_Code_to_Voltage(values[i]));
      }
      g_dma_half_complete = false;
    }
    if (g_dma_complete)
    {
      for (int i = 0; i < 4; i++)
      {
        printf(">>> %s, value : %d, voltage: %.2f\n", channel_name[i ], values[i+4], ADC_Code_to_Voltage(values[i+41]));
      }
    }

    // if(g_dma_complete)
    // {
    //   for(int i = 0; i < 4; i++)
    //   {
    //     printf(">>> %s, value : %d, voltage: %.2f\n",channel_name[i], values[i], ADC_Code_to_Voltage(values[i]));
    //   }
    //   g_dma_complete = false;
    //   HAL_ADC_Stop_DMA(&hadc3);
    //   HAL_ADC_Start_DMA(&hadc3, (uint32_t*)values, 4);
    // }
    // HAL_Delay(3000);

    // ADC_Read_MultipleChannel_Test();
    // HAL_Delay(300);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// DMA 完成中断
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc == &hadc3)
  {
    g_dma_complete = true;
  }
}

//DMA 半完成中断
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc3)
  {
    g_dma_half_complete = true;
  }
}

// static void ADC_Read_MultipleChannel_Test(void)
// {
//   uint32_t values[4] = {0};
//   uint32_t num = sizeof(values) / sizeof(values[0]);
//   ADC_Read_MultipleChannel_Block(&hadc3, values, num);
//   const char* channel_name[] =
//   {
//     "ADC_CHANNEL_11",
//     "ADC_CHANNEL_10"
//     "ADC_CHANNEL_5",
//     "ADC_CHANNEL_4",
//   };
//   printf("-----------------\n");
//   for(int i = 0; i < num; i++)
//   {
//     printf(">>> %s, value : %d, voltage: %.2f\n",channel_name[i], values[i], ADC_Code_to_Voltage(values[i]));
//   }
// }

// static void ADC_Read_MultipleChannel_Block(ADC_HandleTypeDef* hadc, uint32_t* values, uint32_t count)
// {
//   if(!hadc || !values || count == 0) Error_Handler();

//   //启动ADC,触发一轮转化
//   if(HAL_ADC_Start(hadc) != HAL_OK) Error_Handler();

//   //
//   for(int i = 0; i < count; i++)
//   {
//     if (HAL_ADC_PollForConversion(hadc, 100) != HAL_OK) Error_Handler();

//     values[i] = HAL_ADC_GetValue(hadc);
//   }

//   HAL_ADC_Stop(hadc);
// }

static float ADC_Code_to_Voltage(uint32_t adc_value)
{
  return (adc_value * 3.3f) / 4096.9f;
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
