#include "App_IC_Test.h"


// TIM_HandleTypeDef* g_htim_key = NULL;

volatile static app_handler_t g_app_handler;
volatile static app_configure_t g_app_configure;
volatile static app_measure_t g_app_measure; 

static void KeyPressCallBack(uint8_t key_id, key_state state)
{
    if(state == KEY_STATE_PRESS)
    {
        switch(key_id)
        {
            case 0:
                BSP_LED_TogglePin(LED1);
                g_app_configure.is_ultrasonic_ranging_enable = !g_app_configure.is_ultrasonic_ranging_enable;
                break;
            case 1:
                BSP_LED_TogglePin(LED2);
                g_app_configure.is_usart_enable = !g_app_configure.is_usart_enable;               
                break;
            case 2:
                BSP_LED_TogglePin(LED3);
                g_app_configure.is_warning_enable = !g_app_configure.is_warning_enable;
                break;
            case 3:
                break;
            default :
                break;

        }
    }
    else if(state == KEY_STATE_RELEASE)
    {
        
    }
    else
    {

    }
}

static void Ultrasonic_Cale_Distance(void)
{
    if(g_app_measure.measure_done)
    {
        uint32_t pulse_width = g_app_measure.falling_time - g_app_measure.rising_time;
        g_app_measure.distance_cm = pulse_width * SPEED / 2.0f ;
        if(g_app_measure.distance_cm > LIMITUP || g_app_measure.distance_cm < LIMITDOWM)
        {
            g_app_measure.distance_cm = 0;
            // g_app_measure.measure_done = false;
        }
    }
}

void App_SetUp(
    TIM_HandleTypeDef* htim_trigger, 
    TIM_HandleTypeDef* htim_key,
    TIM_HandleTypeDef* htim_ic,
    UART_HandleTypeDef* huart)
{
    if(!htim_trigger || !htim_ic || !htim_key || !huart) 
        return; 

    //句柄指针初始化
    g_app_handler.htim_ic = htim_ic;
    g_app_handler.htim_key = htim_key;
    g_app_handler.htim_trigger = htim_trigger;
    g_app_handler.huart = huart;

    //配置信息初始化
    g_app_configure.is_ultrasonic_ranging_enable = false;
    g_app_configure.is_usart_enable = false;
    g_app_configure.is_warning_enable = false;

    //measure初始化
    g_app_measure.distance_cm = 0.0f;
    g_app_measure.falling_time = 0;
    g_app_measure.rising_time = 0;
    g_app_measure.measure_done = true;


    //初始化串口
    BSP_Usart_Init(g_app_handler.huart);

    //按键初始化
    Key_Init(KeyPressCallBack);

    //开启tim6定时器
    HAL_TIM_Base_Start_IT(g_app_handler.htim_key);

    //开启输入捕获
    HAL_TIM_IC_Start_IT(g_app_handler.htim_ic, TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(g_app_handler.htim_ic, TIM_CHANNEL_4);


    g_app_handler.is_init = true;


    // //获取外部句柄指针
    // g_htim_key = htim_key;

    // //初始化按键
    // Key_Init(KeyPressCallBack);

    // //开启base定时器
    // HAL_TIM_Base_Start_IT(g_htim_key);


}

static void zTrigger_10us_Pulse(void)
{
    //开始测量，重置标志位
    g_app_measure.measure_done = false;

    //重置计数器
    __HAL_TIM_SET_COUNTER(g_app_handler.htim_ic, 0);

    //发脉冲
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);

    //启动tim7，进行计时
    HAL_TIM_Base_Start_IT(g_app_handler.htim_trigger);

}

static void Print_Distance_Log()
{
    if(g_app_measure.distance_cm < LIMITDOWM)
    {
        return;
    }
    printf("%.2f",g_app_measure.distance_cm);
}

static void Warning_LED()
{
    if(g_app_measure.distance_cm < LIMITDOWM)
    {
        return;
    }
    if(g_app_measure.distance_cm < 10)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
    }
}

void App_LoopOnce()
{
    //判断初始化
    if(!g_app_handler.is_init) return;
    //根据配置，执行逻辑
    if(g_app_configure.is_ultrasonic_ranging_enable)
    {
        Trigger_10us_Pulse();
        HAL_Delay(30);//完成一轮超声波测距

        if(g_app_measure.measure_done)
        {
            if(g_app_configure.is_usart_enable)
            {
                Print_Distance_Log();
            }
            if(g_app_configure.is_warning_enable)
            {
                Warning_LED();
            }
        }
    }

    // HAL_Delay(100);
    // printf("%d %d %d\n", g_app_configure.is_ultrasonic_ranging_enable,
    //     g_app_configure.is_usart_enable,g_app_configure.is_warning_enable);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim == g_app_handler.htim_ic)
    {
        if(htim->Channel == TIM_CHANNEL_3)
        {
            g_app_measure.rising_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
            // g_app_measure.measure_done = false;
        }
        else if(htim->Channel == TIM_CHANNEL_4)
        {
            g_app_measure.falling_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
            g_app_measure.measure_done = true;

            Ultrasonic_Cale_Distance();
            
        }
        else
        {

        }
    }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //TIM6超时
    if(htim == g_app_handler.htim_key)
    {
        Key_Scan();
    }

    //TIM7超时
    if(htim == g_app_handler.htim_trigger)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

        //关闭TIM7和中断
        HAL_TIM_Base_Stop_IT(g_app_handler.htim_trigger);
    }
}
