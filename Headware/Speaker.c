#include "stm32f10x.h"
#include "Delay.h"

#define Speak_time 300

void Speaker_Init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	
}

void Speak(void)
{
   GPIO_SetBits(GPIOB, GPIO_Pin_0);
   Delay_ms(Speak_time);
   
   GPIO_ResetBits(GPIOB, GPIO_Pin_0);
   Delay_ms(Speak_time);
	
   GPIO_SetBits(GPIOB, GPIO_Pin_0);
   Delay_ms(Speak_time);
	
   GPIO_ResetBits(GPIOB, GPIO_Pin_0);
   Delay_ms(Speak_time);
    
   GPIO_SetBits(GPIOB, GPIO_Pin_0);
   Delay_ms(Speak_time);
	
   GPIO_ResetBits(GPIOB, GPIO_Pin_0);
   Delay_ms(Speak_time);
}
