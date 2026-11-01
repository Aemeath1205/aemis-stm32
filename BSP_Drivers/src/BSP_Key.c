#include "BSP_Key.h"

volatile key_t keys[KEY_NUM];
volatile KeyCallBack_t g_key_callback = NULL;

void Key_Init(KeyCallBack_t cb)//?
{
    for(int i = 0; i < KEY_NUM; i++)
    {
        keys[i].state = KEY_STATE_RELEASE;
        keys[i].debounce_cnt = 0;
    }
    g_key_callback = cb;
}

//检测四个按键的高低电平,使用编号依次检测
//0 按下  1 断开
//0,1,2,3，表示四个按键
static uint8_t ReadKey(uint8_t key_id)
{
    switch(key_id)
    {
        case 0 : return HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN);
        case 1 : return HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN);
        case 2 : return HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN);
        case 3 : return HAL_GPIO_ReadPin(KEY3_PORT, KEY3_PIN);
        default: return 1;
    }
}

//扫描所有按键的函数
//未来被定时器中断调用
void Key_Scan(void)
{
    //遍历所有按键
    for(int index = 0; index < KEY_NUM; index++)
    {
        //读取特定引脚的电平，判断是否被按下
        uint8_t level = ReadKey(index);

        //按下
        if(level == KEY_ACTIVE_LEVEL)
        {
            //按下消抖（计数器）
            if(keys[index].debounce_cnt < DEBOUNCE_MS)
            {
                keys[index].debounce_cnt++;//本质在延时
            }
            //缺失按下了
            if(keys[index].debounce_cnt >= DEBOUNCE_MS && keys[index].state == KEY_STATE_RELEASE)
            {
                keys[index].state = KEY_STATE_PRESS;
                if(g_key_callback != NULL) g_key_callback(index, KEY_STATE_PRESS);
            }
        }
        else // ?
        {
            //弹起状态
            if(keys[index].debounce_cnt > 0)
            {
                keys[index].debounce_cnt--;
            }
            //按键松开||默认状态
            if(keys[index].debounce_cnt == 0 && keys[index].state == KEY_STATE_PRESS)
            {
                keys[index].state = KEY_STATE_RELEASE; 
                if(g_key_callback != NULL) g_key_callback(index, KEY_STATE_RELEASE);

            }

        }
    }
}