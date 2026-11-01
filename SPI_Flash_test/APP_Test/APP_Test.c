#include "APP_Test.h"

void Test_W25Q24_Driver_Init(SPI_HandleTypeDef* hspi, 
                        GPIO_TypeDef* port, uint16_t pin)
{
    printf("--------------Test_W25Q24_Driver_Init--------------\n");
    spi_flash_t init = 
    {
        .page_size      = 256                ,
        .sector_size    = 256 * 16           ,
        .block_size     = 256 * 16 * 16      ,
        .mem_total_size = 256 * 16 * 16 * 128,
        .chip_id        = 0xEF4017           ,
        .hspi           = hspi               ,
        .gpio_port      = port               ,
        .nss_pin        = pin                
    };
    BSP_W25Qxx_Init(&init);
}

void Test_W25Q24_Driver_Reg1(void)
{
    printf("--------------Test_W25Q24_Driver_Reg1--------------\n");
    uint8_t reg1 = BSP_W25Qxx_Statue1_Reg_1();
    printf(">> Reg1 is 0x%x\n",reg1);
}

void Test_W25Q24_Driver_Write_Enable_Disable(void)
{   
    printf("----------Test_W25Q24_Driver_Write_Enable----------\n");
    uint8_t reg1 = BSP_W25Qxx_Statue1_Reg_1();
    printf(">> Reg1 is 0x%x\n",reg1);
    
    BSP_W25Qxx_Write_Enable();

    reg1 = BSP_W25Qxx_Statue1_Reg_1();
    printf(">> Reg1 is 0x%x\n",reg1);

    BSP_W25Qxx_Write_Disable();

    reg1 = BSP_W25Qxx_Statue1_Reg_1();
    printf(">> Reg1 is 0x%x\n",reg1);
}

void Test_W25Q24_Driver_Erase_Read(void)
{
    printf("--------------Test_W25Q24_Driver_Erase_Read--------------\n");
    uint32_t start_address   = 0;
    uint32_t end_address     = 4096-4;
    uint32_t rx_buffer_first = 0;
    uint32_t rx_buffer_last  = 0;

    uint32_t data = 0;
    BSP_W25Qxx_Page_Program(0, 0, (uint8_t*)&data, sizeof(data));

    BSP_W25Qxx_Read_Data(start_address, (uint8_t*)&rx_buffer_first, sizeof(rx_buffer_first));
    BSP_W25Qxx_Read_Data(end_address  , (uint8_t*)&rx_buffer_last , sizeof(rx_buffer_last) );
    printf(">>擦除前 rx_buffer_first : 0x%x, rx_buffer_last : 0x%x\n", rx_buffer_first, rx_buffer_last);
   
    BSP_W25Qxx_Sector_Erase(start_address);
    // BSP_W25Qxx_Wait_For_Write_Complete_V1();

    rx_buffer_first = 0;
    rx_buffer_last  = 0;

    BSP_W25Qxx_Read_Data(start_address, (uint8_t*)&rx_buffer_first, sizeof(rx_buffer_first));
    BSP_W25Qxx_Read_Data(end_address  , (uint8_t*)&rx_buffer_last , sizeof(rx_buffer_last) );
    printf(">>擦除后 rx_buffer_first : 0x%x, rx_buffer_last : 0x%x\n", rx_buffer_first, rx_buffer_last);

}

uint8_t rbuffer[4096*2];

static void printdata(int step)
{
    printf("----------step:%d-----------\n",step);
    for(int i = 0;i < 8; i++)
    {
        printf("0x%x ",rbuffer[i]);
    }
    printf("\n");
    for(int i = 4096*2-8;i < 4096*2; i++)
    {
        printf("0x%x ",rbuffer[i]);
    }
    printf("\n");
}


void Test_W25Q24_Driver_Chiperase_Any_Read_Write(void)
{
    uint32_t start_address = 0x0;
    uint16_t size          = 4096*2;

    BSP_W25Qxx_Read_Data(start_address,rbuffer,size);

    printdata(1);

    BSP_W25Qxx_Sector_Erase(0);
    BSP_W25Qxx_Sector_Erase(4096);
    BSP_W25Qxx_Read_Data(start_address,rbuffer,size);

    printdata(2);

    memset(rbuffer,0,sizeof(rbuffer));

    BSP_W25Qxx_Write_Data(start_address,rbuffer,size);
    BSP_W25Qxx_Read_Data(start_address,rbuffer,size);

    printdata(3);


    BSP_W25Qxx_Chip_Erase();

    BSP_W25Qxx_Read_Data(start_address,rbuffer,size);
    printdata(4);
}