#include "stm32f10x.h"
#include "Delay.h"

#define  DLA GPIO_Pin_2
#define  DLB GPIO_Pin_3
#define  DLE GPIO_Pin_0

#define  DRA GPIO_Pin_4
#define  DRB GPIO_Pin_5 
#define  DRE GPIO_Pin_1

#define Detect_Pin1 GPIO_Pin_8
#define Detect_Pin2 GPIO_Pin_9
#define Detect_Pin3 GPIO_Pin_10
#define Detect_Pin4 GPIO_Pin_11

void Driver_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
     
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = DLA|DLB|DRA|DRB;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Detect_Pin1|Detect_Pin2|Detect_Pin3|Detect_Pin4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOA,DLA|DLB|DRA|DRB,Bit_SET);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =  DLE|DRE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);

    TIM_InternalClockConfig(TIM2);
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
	
}

void DLA_W(uint8_t Bit)
{
  GPIO_WriteBit(GPIOA,DLA,(BitAction)Bit);
}

void DRA_W(uint8_t Bit)
{
  GPIO_WriteBit(GPIOA,DRA,(BitAction)Bit);
}

void DLB_W(uint8_t Bit)
{
  GPIO_WriteBit(GPIOA,DLB,(BitAction)Bit);
}

void DRB_W(uint8_t Bit)
{
  GPIO_WriteBit(GPIOA,DRB,(BitAction)Bit);
}

void DL_Spead(uint16_t spead)
{  
  TIM_SetCompare1(TIM2,spead);
}

void DR_Spead(uint16_t spead)
{  
  TIM_SetCompare2(TIM2,spead);
}
	
void DL_Go(uint16_t spead)
{
   DLA_W(1);
   DLB_W(0);
   DL_Spead(spead);
}

void DR_Go(uint16_t spead)
{
   DRA_W(1);
   DRB_W(0);
   DR_Spead(spead);
}

void DL_Stop(void)
{
   DLA_W(0);
   DLB_W(0);
}

void DR_Stop(void)
{
   DRA_W(0);
   DRB_W(0);
}

void DL_Back(uint16_t spead)
{   
   DLA_W(0);
   DLB_W(1);
   DL_Spead(spead);

}

void DR_Back(uint16_t spead)
{
   DRA_W(0);
   DRB_W(1);
   DR_Spead(spead);
}

void D_GO(uint16_t spead)
{
  DL_Go(spead);
  DR_Go(spead);
}

void D_Stop(void)
{
  DL_Stop();
  DR_Stop();
}
void D_TurnRight(uint16_t spead)
{
  DL_Go(spead);
  DR_Back(spead);
}
void D_TurnLeft(uint16_t spead)
{
  DL_Back(spead);
  DR_Go(spead);
}

uint8_t Get_Detect1Value(void)
{
   return GPIO_ReadInputDataBit(GPIOA,Detect_Pin1);
}                                              
                                               
uint8_t Get_Detect2Value(void)                 
{                                              
   return GPIO_ReadInputDataBit(GPIOA,Detect_Pin2);
}                                               
                                                
uint8_t Get_Detect3Value(void)                  
{                                               
   return GPIO_ReadInputDataBit(GPIOA,Detect_Pin3);
}                                               
                                                
uint8_t Get_Detect4Value(void)                  
{                                               
   return GPIO_ReadInputDataBit(GPIOA,Detect_Pin4);
}

uint8_t Get_DetectValue(void)
{
   uint8_t GetValue = 0x00; //0000
     GetValue |= (Get_Detect1Value()<<3);//1000
     GetValue |= (Get_Detect2Value()<<2);//1100
	 GetValue |= (Get_Detect3Value()<<1);//1110
	 GetValue |= Get_Detect4Value();//1111
	return GetValue;
}

uint8_t Get_Spead(void)
{
   return TIM_GetCounter(TIM2);
}
	
	

