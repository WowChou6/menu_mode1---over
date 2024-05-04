#include "stm32f10x.h"
#include "LED.h"


void Serial_Init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed =  GPIO_Speed_50MHz;
	
    GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed =  GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitTypeDef USARRT_InitStruture;
	USARRT_InitStruture.USART_BaudRate = 9600;
	USARRT_InitStruture.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
	USARRT_InitStruture.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USARRT_InitStruture.USART_Parity = USART_Parity_No;
	USARRT_InitStruture.USART_StopBits = USART_StopBits_1;
	USARRT_InitStruture.USART_WordLength  = USART_WordLength_8b;
	
	USART_Init(USART2,&USARRT_InitStruture);
	
	USART_Cmd(USART2, ENABLE);
}


void Serial_SendData(uint16_t Byte)
{
    USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
}


