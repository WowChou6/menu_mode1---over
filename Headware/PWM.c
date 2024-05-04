#include "stm32f10x.h"

uint8_t num = 0;
void PWM_Init(uint32_t Freq)
{
	//72M 4800K 
  uint32_t	ARR = 4800000/Freq;
	
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = ARR-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 15-1; 
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
    TIM_Cmd(TIM2,ENABLE);

	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = ARR/2;
	TIM_OC1Init(TIM2,&TIM_OCInitStruct);

}


void PWM_SetCompare1(uint8_t Compare)
{
    TIM_SetCompare1(TIM2, Compare);
}
 
void PWM_SetCompare2(uint8_t Compare)
{
    TIM_SetCompare2(TIM2, Compare);
}

void PWM_SetCompare3(uint8_t Compare)
{
    TIM_SetCompare3(TIM2, Compare);
}


void PWM_Stepping(void)
{  
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
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
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC2Init(TIM2,&TIM_OCInitStruct);
}

void IC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 65535;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct); 
	 
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 1;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);
	
	TIM_PWMIConfig(TIM2,&TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Reset);
	TIM_Cmd(TIM2,ENABLE);
}


uint32_t IC_GetFreq(void)
{
  return 1000000/ (TIM_GetCapture1(TIM2)+1);
}

uint32_t IC_GetDuty(void)
{
 return (TIM_GetCapture2(TIM2)+1)*100/(TIM_GetCapture1(TIM2)+1);
}
