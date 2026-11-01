#include "APP.h"

// 全局配置变量
_config_t gconfig;

static void Config_Display(void)
{
    printf(">>--------------配置清单---------------<<\n");
    printf("gconfig.magic:0x%x\n"   ,gconfig.magic);
    printf("gconfig.version:0x%x\n" ,gconfig.version);
    printf("gconfig.led_id:%d\n"    ,gconfig.led_id);
    printf("gconfig.repeat_cnt:%d\n",gconfig.repeat_cnt);
    printf("gconfig.delay_ms:%d\n"  ,gconfig.delay_ms);
    printf("gconfig.start_time:%d\n",gconfig.start_time);
}

// ret == EOF 读取失败
static int Request_Input(const char *message)
{
    int value = 0;
    printf(">> %s\n", message);
    if (scanf("%d", &value) == 1)
    {
        return value;
    }
    printf("[!]输入无效，请重试\n");
    return EOF;
}

void APP_Init(void)
{
    // 获取用户选择 1.重新配置参数 2.使用现有的配置
    uint8_t choice = Request_Input("1.重新配置参数 2.使用现有的配置");
    if (choice == 1)
    {
        gconfig.magic      =  APP_MAGIC;
        gconfig.version    =  APP_VERSION;
        gconfig.led_id     = (uint8_t)Request_Input("step1:选择LED[1,4],5表示全选");
        gconfig.repeat_cnt = (uint8_t)Request_Input("step2:设置闪灯次数,[1,255]");
        gconfig.delay_ms   = (uint16_t)Request_Input("step3:设置闪灯间隔,单位ms>>");
        gconfig.start_time = (uint16_t)Request_Input("step4:设置开始时间,单位ms>>");

        //写入EEPROM
        Config_Display();
        SAVE_CONFIG(&gconfig, sizeof(gconfig));
        printf(">> write config down");
    }
    else if (choice == 2)
    {
        //读取EEPROM
        LOAD_CONFIG(&gconfig, sizeof(gconfig));
        printf(">> load config down");

    }
    else
    {
        printf("[!]输入无效,选项只有1和2!\n");
    }
}

void APP_Run(void)
{
    if(gconfig.magic == APP_MAGIC && gconfig.version == APP_VERSION)
    {
        printf(">>%d ms后,开始闪灯\n",(int)(gconfig.start_time));
        HAL_Delay(gconfig.start_time);
        printf(">>闪灯中...\n");
        LED_Blink((LED_t)gconfig.led_id , gconfig.repeat_cnt , gconfig.delay_ms);
    }
}