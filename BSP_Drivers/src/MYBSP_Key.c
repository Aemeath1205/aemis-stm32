#include "MYBSP_Key.h"

volatile Single_key_t Keys[LEDNUMS] = { 0 };
volatile KeyCallBack_t keycallback = NULL;

void Key_Init(KeyCallBack_t cb)
{
    for(int i = 0; i < LEDNUMS; i++)
    {
        Keys[i].key_id       = (Key_id_t)(i+1);
        Keys[i].key_state    = State_Release;
        Keys[i].is_init      = true;
        Keys[i].debounce_cnt = 0;
    }
    keycallback = cb;
}

static Key_state_t Single_Key_Scan(uint8_t key) // true pressed false release
{
    switch(key)
    {
        case 1:
            if(HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == GPIO_PIN_RESET) return State_Pressed;
            else return State_Release;
        case 2:
            if(HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN) == GPIO_PIN_RESET) return State_Pressed;
            else return State_Release;
        case 3:
            if(HAL_GPIO_ReadPin(KEY3_PORT, KEY3_PIN) == GPIO_PIN_RESET) return State_Pressed;
            else return State_Release;
        case 4:
            if(HAL_GPIO_ReadPin(KEY4_PORT, KEY4_PIN) == GPIO_PIN_RESET) return State_Pressed;
            else return State_Release;
        default:
            return State_Release;
    }
}

void Key_Scan(void)
{
    for(int i = 1; i <= LEDNUMS; i++)
    {
        Key_state_t key_state = Single_Key_Scan(i);
        if(key_state == State_Pressed)
        {
            if(Keys[i-1].debounce_cnt < DEBOUNCECNT) Keys[i-1].debounce_cnt++;
            if(Keys[i-1].debounce_cnt >= DEBOUNCECNT && Keys[i-1].key_state == State_Release)
            {
                Keys[i-1].key_state = State_Pressed;
                if(keycallback != NULL) keycallback(i, State_Pressed);
            }
        }
        else
        {
            if(Keys[i-1].debounce_cnt > 0) Keys[i-1].debounce_cnt--;
            if(Keys[i-1].debounce_cnt == 0 && Keys[i-1].key_state == State_Pressed)
            {
                Keys[i-1].key_state = State_Release;
                if(keycallback != NULL) keycallback(i, State_Release);
            }
        }
    }
}
