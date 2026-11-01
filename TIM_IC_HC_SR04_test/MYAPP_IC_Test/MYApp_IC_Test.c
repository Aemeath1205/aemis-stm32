#include "MYApp_IC_Test.h"

volatile App_handle_t    App_handle = {0};
volatile App_configure_t App_configure = {0};
volatile App_data_t      App_data = {0};

static void KeyCallBack(uint8_t key_id, Key_state_t key_state)
{
    if(key_state == State_Pressed)
    {
        switch(key_id)
        {
            case 1:
                BSP_LED_TogglePin(LED1);
                App_configure.is_trigger_enable = !App_configure.is_trigger_enable;
                break;
            case 2:
                BSP_LED_TogglePin(LED2);
                App_configure.is_usart_enable = !App_configure.is_usart_enable;
                break;
            case 3:
                BSP_LED_TogglePin(LED4);
                App_configure.is_warningled_enable = !App_configure.is_warningled_enable;
                break;
            default:
        }
    }
}

void App_SetUp(
    TIM_HandleTypeDef* htim_trigger, 
    TIM_HandleTypeDef* htim_key,
    TIM_HandleTypeDef* htim_ic,
    UART_HandleTypeDef* huart)
{
    if(!htim_ic || !htim_key || !htim_trigger || !huart) return;

    App_configure.is_trigger_enable    = false;
    App_configure.is_usart_enable      = false;
    App_configure.is_warningled_enable = false;

    App_handle.htim_ic      = htim_ic;
    App_handle.htim_key     = htim_key;
    App_handle.htim_trigger = htim_trigger;
    App_handle.huart        = huart;

    App_data.rising_time     = 0;
    App_data.failing_time    = 0;
    App_data.final_distance  = 0.0f;
    App_data.is_measure_done = true;

    Key_Init(KeyCallBack);
    BSP_Usart_Init(huart);

    HAL_TIM_Base_Start_IT(App_handle.htim_key);

    HAL_TIM_IC_Start_IT(App_handle.htim_ic,TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(App_handle.htim_ic,TIM_CHANNEL_4);

    App_handle.Is_Init = true;
}

static void Send()
{
    App_data.is_measure_done = false;
    __HAL_TIM_SET_COUNTER(App_handle.htim_ic, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    HAL_TIM_Base_Start_IT(App_handle.htim_trigger);
}

void App_LoopOnce(void)
{
    if(!App_handle.Is_Init) return;
    if(!App_configure.is_trigger_enable) return;
    Send();
    HAL_Delay(30);
    if(App_data.is_measure_done)
    {
        if(App_configure.is_usart_enable)
        {
            if(App_data.final_distance < LIMITDOWN || App_data.final_distance > LIMITUP)
                return;
            printf(">>> %.2f", App_data.final_distance);
        }
        if(App_configure.is_warningled_enable)
        {
            if(App_data.final_distance > LIMITDOWN && App_data.final_distance < 10)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
            }
        }
    }

}

static void distance()
{
    if(!App_data.is_measure_done) return;
    uint32_t width = App_data.failing_time - App_data.rising_time;
    App_data.final_distance = width * SPEED / 2.0f;
    if(App_data.final_distance < LIMITDOWN || App_data.final_distance > LIMITUP)
    {
        App_data.final_distance = 0.0f;
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim == App_handle.htim_ic)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            App_data.is_measure_done = false;
            App_data.rising_time     = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
        }
        else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
        {
            App_data.is_measure_done = true;
            App_data.failing_time    = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
            distance();
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == App_handle.htim_key)
    {
        Key_Scan();
    }
    if(htim == App_handle.htim_trigger)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        HAL_TIM_Base_Stop_IT(htim);
    }
}