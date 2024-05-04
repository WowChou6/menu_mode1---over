#include "stm32f10x.h"
#include "Delay.h"

int key0 = 1;
int key1 = 1;

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
}

uint8_t Key0_read(void)
{ 
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == RESET){
    Delay_ms(10);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == RESET);
	Delay_ms(10);
    key0 = 0;
    }
	else key0 = 1;
	
return key0;
}

uint8_t Key1_read(void)
	{ 
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == RESET){
    Delay_ms(10);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == RESET);
	Delay_ms(10);
    key1 = 0;
    }
	else key1 = 1;
	
return key1;
}
