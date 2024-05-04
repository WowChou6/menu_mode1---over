#include "stm32f10x.h"


float Velocity_KP = 1500,Velocity_KI = 50;
float TKd = 25000,TKp = 2000;

void Encoder_Init(void)
{
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//编码器输出引脚配置
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	//PWM输出引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM4);
	
//	//电机转动方向引脚配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);	
//	
//	//电机一直正转
//	GPIO_ResetBits(GPIOA,GPIO_Pin_9|GPIO_Pin_10);
//	GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_11);
//	
	//编码器接口配置
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 65535;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct); 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct); 
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//PWM输出配置
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 65535;      //修改点 
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0; 
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);
	TIM_OC2Init(TIM4,&TIM_OCInitStruct);
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);
	TIM_OC4Init(TIM4,&TIM_OCInitStruct);
	
	//使能时基
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}

int16_t Encoder1_Get(void)
{
	int16_t Speed_Get = TIM_GetCounter(TIM2);
	TIM_SetCounter(TIM2,0);
    return Speed_Get;
}

int16_t Encoder2_Get(void)
{
	int16_t Speed_Get = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
    return Speed_Get;
	
}

void L_Go(int num)
{
   if(num == 1)
   {
      TIM_SetCompare1(TIM4, 0);
   }
   else
   {
	  TIM_SetCompare2(TIM4, 0);
   }
}

void R_Go(int num)
{
    if(num == 1)
	{
		
	 TIM_SetCompare3(TIM4, 0);
		
	}
	else
	{
		
	 TIM_SetCompare4(TIM4, 0);
		
	}	
}


int abs(int num){
  if(num < 0) return -num;
	else return num;
}


//写入PWM
void PWM_Set(int16_t PWM1,int16_t PWM2)
{
    TIM_SetCompare1(TIM4, PWM1);
	TIM_SetCompare2(TIM4, PWM2);
}


void set_L_pwm(int PWM1)
{
	if(PWM1 >= 0) 
	{
	   L_Go(1);
	   TIM_SetCompare2(TIM4, PWM1);
	}
	else {
	   L_Go(0);
	   TIM_SetCompare1(TIM4, -PWM1);
	}  
}

void set_R_pwm(int PWM2)
{
	if(PWM2 >= 0) 
	{
	   TIM_SetCompare4(TIM4, 0);
	   TIM_SetCompare3(TIM4,PWM2);
	}
	else {
	   TIM_SetCompare3(TIM4, 0);
	   TIM_SetCompare4(TIM4, -PWM2);
	}  
}

//定时中断初始化
void Timer_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
    TIM_InternalClockConfig(TIM1);
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 20 - 1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 36000 - 1; 
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1,ENABLE);
}


int xunji(int huidu_data)
{
	//int H[8];
	//H[0] = huidu_data >> 7;
	//H[1] = (huidu_data & 0x40) >> 6;
	//H[2] = (huidu_data & 0x20) >> 5;
	//H[3] = (huidu_data & 0x10) >> 4;
	//H[4] = (huidu_data & 0x8) >> 3;
	//H[5] = (huidu_data & 0x4) >> 2;
	//H[6] = (huidu_data & 0x2) >> 1;
	//H[7] = huidu_data & 0x1;
	
	int pos=0;
	static int last_pos;

	switch (huidu_data)
	{
	case 0x10:/*10000*/ pos = 4; break;
	case 0x18:/*11000*/ pos = 3; break;
	case 0x8:/*01000*/ pos = 2; break;
	case 0xc:/*01100*/ pos = 1; break;
	case 0x4:/*00100*/ pos = 0; break;
	case 0x6: /*00110*/ pos = -1; break;
	case 0x2: /*00010*/ pos = -2; break;
	case 0x3: /*00011*/ pos = -3; break;
	case 0x1: /*00001*/ pos = -4; break;
		
	//交叉口返回100
	case 0x1c:/*11100*/
	case 0x1e:/*11110*/
	case 0x07:/*00111*/
	case 0x0f:/*01111*/
		
	case 0x1f:/*11111*/ return 100;
	default: pos = last_pos;
	}

	last_pos = pos;
	return pos;
}	


//左速度环  （增量式）
//输入：左轮实际速度，左轮目标速度
//输出：左轮电机pwm
int Velocity_L(int L_vel, int L_vel_target)
{
	static float L_vel_bias, L_vel_pwm, L_vel_last_bias;
	L_vel_bias = L_vel_target - L_vel;
	L_vel_pwm += Velocity_KP * (L_vel_bias - L_vel_last_bias) + Velocity_KI * L_vel_bias;
	L_vel_last_bias = L_vel_bias;
	return L_vel_pwm;
}

//右速度环  （增量式）
//输入：右轮实际速度，右轮目标速度
//输出：右轮电机pwm
int Velocity_R(int R_vel, int R_vel_target)
{
	static float R_vel_bias, R_vel_pwm, R_vel_last_bias;
	R_vel_bias = R_vel_target - R_vel;
	R_vel_pwm += Velocity_KP * (R_vel_bias - R_vel_last_bias) + Velocity_KI * R_vel_bias;
	R_vel_last_bias = R_vel_bias;
	return R_vel_pwm;
}

//转向
//匀速转向，当快达到目标位置时，刹停，靠惯性甩到目标位置
//输入：轮实际位置，轮目标位置（左右轮对称）
//输出：电机pwm
int Turn(int turn_pos, int turn_target)
{
	if (turn_target > 0)
	{
		if (turn_pos < turn_target - 150) return 50000;
		else return -5000;
	}
	else
	{
		if ( abs(turn_pos) < abs(turn_target) - 150) return -55000;
		else return 5000;
	}
}

int MTurn(int turn_pos, int turn_target)
{
	
	static float T_Bias,T_Pwm,Last_T_Bias;
	
	T_Bias = turn_target - turn_pos;
	
	T_Pwm = TKd * (T_Bias - Last_T_Bias) + TKp * T_Bias;
	
	Last_T_Bias = T_Bias;
	if(T_Pwm >= 53000) T_Pwm = 53000;
	if(T_Pwm <= -53000) T_Pwm = -53000;
	
   return T_Pwm;
	
	
}

void brake(void)
{
     L_Go(0);
	 L_Go(1);
	 R_Go(1);
	 R_Go(0);
}
