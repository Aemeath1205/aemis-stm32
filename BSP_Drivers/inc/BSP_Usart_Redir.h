#ifndef __BSP_USART_REDIR_H
#define __BSP_USART_REDIR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef *ghusart;
void BSP_Usart_Init(UART_HandleTypeDef *husart);


#ifdef __cplusplus
}
#endif


#endif