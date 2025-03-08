#include "stm32f4xx_hal.h"

#ifndef __SERIAL_H
#define __SERIAL_H

#define HEAD_1					0xFF
#define HEAD_2					0xFE
#define TAIL					'\n'
#define SIZE					8
#define RX_Control_User			8
#define RX_Control_User_add		2*RX_Control_User

typedef enum
{
	Serial_OK,
	Serial_ERROR,
	
}Serial_State;

//void Serial_Init(UART_HandleTypeDef *huart, uint8_t *DstAddress, uint8_t *SecondMemAddress, uint32_t DataLength);
void USART_DMAEx_MultiBuffer_Init(UART_HandleTypeDef *huart, uint32_t *DstAddress, uint32_t *SecondMemAddress, uint32_t DataLength);


#endif
