#ifndef __APP_TEST_H
#define __APP_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "BSP_W25Q64.h"

void Test_W25Q24_Driver_Init(SPI_HandleTypeDef* hspi, 
                        GPIO_TypeDef* port, uint16_t pin);

void Test_W25Q24_Driver_Reg1(void);
void Test_W25Q24_Driver_Write_Enable_Disable(void);
void Test_W25Q24_Driver_Erase_Read(void);
void Test_W25Q24_Driver_Chiperase_Any_Read_Write(void);


#ifdef __cplusplus
}
#endif

#endif