//#include <string.h>
#include "Serial.h"
#include "Servo_front.h"

extern UART_HandleTypeDef huart4;
extern volatile uint8_t control_rx_buf[2][RX_Control_User];

uint16_t buffer_index = 0;


void USART_DMAEx_MultiBuffer_Init(UART_HandleTypeDef *huart, uint32_t *DstAddress, uint32_t *SecondMemAddress, uint32_t DataLength)
{
 huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;

 huart->RxEventType = HAL_UART_RXEVENT_IDLE;

 huart->RxXferSize    = DataLength;

 SET_BIT(huart->Instance->CR3,USART_CR3_DMAR);

 __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE); 
 
do{
      __HAL_DMA_DISABLE(huart->hdmarx);
  }while(((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR & DMA_SxCR_EN);

((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->PAR = (uint32_t)&huart->Instance->DR;

((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->M0AR = (uint32_t)DstAddress;

((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->M1AR = (uint32_t)SecondMemAddress;

((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->NDTR = DataLength;

SET_BIT(((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR, DMA_SxCR_DBM);

__HAL_DMA_ENABLE(huart->hdmarx);	
}

//static void USART_RxDMA_DoubleBuffer_Init(UART_HandleTypeDef *huart, uint32_t *DstAddress, uint32_t *SecondMemAddress, uint32_t DataLength)
//{ 

// huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE; 

// huart->RxEventType = HAL_UART_RXEVENT_IDLE; 

// huart->RxXferSize    = DataLength; 

// SET_BIT(huart->Instance->CR3,USART_CR3_DMAR); 

// __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);  
// 
// HAL_DMAEx_MultiBufferStart(huart->hdmarx,(uint32_t)&huart->Instance->DR,(uint32_t)DstAddress,(uint32_t)SecondMemAddress,DataLength); 
//}

uint16_t __bytes_to_uint16(uint8_t byte1, uint8_t byte2)
{
    // 将高字节左移8位，然后与低字节进行按位或操作
    return ((uint16_t)byte1 << 8) | (uint16_t)byte2;
}

Serial_State __Data_Processing(volatile uint8_t control_rx_buf[])
{	
	if ((control_rx_buf[0] == 0xFF) && (control_rx_buf[7] == 0x0F))
	{		
		if (control_rx_buf[1] == 0x00)
		{
			Servo_Front_All_Up();
		}
		
		else if (control_rx_buf[1] == 0x00)
		{
			Servo_Front_All_Down();
		}
		
		else if (control_rx_buf[1] == 0x00)
		{
			Servo_Front_Single((Servo_Front_Num)control_rx_buf[2], __bytes_to_uint16(control_rx_buf[3], control_rx_buf[4]));
		}
		else if (control_rx_buf[1] == 0x00)
		{
			Servo_360((Servo_360_Direction)control_rx_buf[2], (Servo_360_Speed)control_rx_buf[3]);
		}
		else
			return Serial_ERROR;
	}
	else
	{
		return Serial_ERROR;
	}
		
	return Serial_OK;
}

void USER_USART4_RxHandler(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(((((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR) & DMA_SxCR_CT ) == RESET)
    {
        __HAL_DMA_DISABLE(huart->hdmarx);

        ((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR |= DMA_SxCR_CT;

        __HAL_DMA_SET_COUNTER(huart->hdmarx,RX_Control_User_add);

        if(Size == RX_Control_User)
        {
			buffer_index = 0;
            __Data_Processing((uint8_t*)control_rx_buf[0]);
			for (int i = 0; i < 10000; i++);
        }
    }
    else
    {
        __HAL_DMA_DISABLE(huart->hdmarx);

        ((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR &= ~(DMA_SxCR_CT);

        __HAL_DMA_SET_COUNTER(huart->hdmarx,RX_Control_User_add);

        if(Size == RX_Control_User)
        {
			buffer_index = 1;
            __Data_Processing((uint8_t*)control_rx_buf[1]);
			for (int i = 0; i < 10000; i++);
        }
    }
    __HAL_DMA_ENABLE(huart->hdmarx);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart == &huart4)
    {
        USER_USART4_RxHandler(huart,Size);
    }
}

//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//{	
//}


