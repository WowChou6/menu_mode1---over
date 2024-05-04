#include "stm32f10x.h"
#include "PWM.h"
#include "Delay.h"

void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_SET);
	
};

void LED_PWM_Init(void)
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 100-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
    TIM_Cmd(TIM2,ENABLE);
	
	 TIM_OCInitTypeDef TIM_OCInitStruct;
	 TIM_OCStructInit(&TIM_OCInitStruct);
	
     TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	 TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	 TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCInitStruct.TIM_Pulse = 100-1;
	 TIM_OC1Init(TIM2,&TIM_OCInitStruct);
	 TIM_OC2Init(TIM2,&TIM_OCInitStruct);
	 TIM_OC3Init(TIM2,&TIM_OCInitStruct);
}

void LED_OFF(void)
{
   GPIO_SetBits(GPIOA,GPIO_Pin_0);
}

void LED_ON(void)
{
  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

void LED_Turn(void)
{
   if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_0) == SET)  ;
   else LED_OFF();
}


void LED1_PWM(uint8_t ms)
{
  for(uint8_t i = 100; i > 0;i--)
	{
		PWM_SetCompare1(i);
		Delay_ms(ms);
	}
  for(uint8_t i = 0; i < 100;i++)
	{
		PWM_SetCompare1(i);
		Delay_ms(ms);
	}
}

void LED2_PWM(uint8_t ms)
{
  for(uint8_t i = 100; i > 0;i--)
	{
		PWM_SetCompare2(i);
		Delay_ms(ms);
	}
  for(uint8_t i = 0; i < 100;i++)
	{
		PWM_SetCompare2(i);
		Delay_ms(ms);
	}
}

void LED3_PWM(uint8_t ms)
{
  for(uint8_t i = 100; i > 0;i--)
	{
		PWM_SetCompare3(i);
		Delay_ms(ms);
	}
  for(uint8_t i = 0; i < 100;i++)
	{
		PWM_SetCompare3(i);
		Delay_ms(ms);
	}
}

