#include "stm32f10x.h"
#include "Encoder.h"
int16_t huidu[3];

float velocity_sum = 0;
int Speed_Pwm1,Speed_Pwm2,div_Pwm,Speed1,Speed2;
int Turn_Pwm,TBias,sum;
static int Speed_err_sum,Last_TBias = 0,Last_Speed_err = 0;
	
void huidu_Init(void) 
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
                       
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_4|GPIO_Pin_5;  // 循迹读取口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
}

int huidu_detect(void)
{
	int data=0;
	//识别到黑输出低电平 白输出高电平
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 1) data = data | 0x10;  //H4
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 1) data = data | 0x8;   //H5
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 1) data = data | 0x4;   //H6
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1) data = data | 0x2;  //H7
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1)  data = data | 0x1;  //H8
	return data;
}







//速度环已调成功
void I_xianfu(void)
{
   if(Speed_err_sum > 2000)
   {
	   Speed_err_sum = 2000;
   }
  
   else if(Speed_err_sum < -2000)
   {
	   Speed_err_sum = -2000;
   }
}
void Speed_xianfu(void)
{
   if(Speed_Pwm1 >= 65535) Speed_Pwm1 = 65535;
   else if(Speed_Pwm1 <= 0) {
	   Speed_Pwm1 = 0;
   }
   if(Speed_Pwm2 >= 65535) Speed_Pwm2 = 65535;
   else if(Speed_Pwm2 <= 0) {
	   Speed_Pwm2 = 0;
   }
}

void Speed_PID(int Speed_exp)
{
	float Skp = 2000,Ski= 50,Skd = 0;//200
	float Dkp = 1800;
  Speed1 = Encoder1_Get();
  Speed2 = Encoder2_Get();
 int Bias;
if(Speed1 > 200) Speed1 = 0;
if(Speed2 > 200) Speed2 = 0;
	
int Speed_err;
	
int Speed = (Speed1 + Speed2)/2;
	
Speed_err = Speed_exp - Speed;
Speed_err_sum += Speed_err;

 I_xianfu();
	
sum = Speed_err_sum;
	
int Speed_Pwm0 = Speed_err * Skp + Speed_err_sum * Ski - Skd * (Speed_err - Last_Speed_err);
	Last_Speed_err = Speed_err;
	
if(Speed1 > Speed2)
{
   Bias = Speed1- Speed2;
   div_Pwm = Bias * Dkp;
  Speed_Pwm1 = Speed_Pwm0 - div_Pwm;
  Speed_Pwm2 = Speed_Pwm0 + div_Pwm; 
}

else
{
   Bias = Speed2- Speed1;
   div_Pwm = Bias * Dkp;
  Speed_Pwm1 = Speed_Pwm0 + div_Pwm;
  Speed_Pwm2 = Speed_Pwm0 - div_Pwm; 
}


}


//转向环
void Turn_PID(int16_t value1,int16_t value2)
{
  if(value1 <= 190) value1 = 190;
  if(value2 <= 190) value2 = 190;
	
    Speed_PID(30);
   	float Tkp = 200,Tkd = 0;//15
	
	
//如果value1 > value2,说明车身向右偏转，此时左轮减速右轮加速
	if(value1 > value2)
{  
	    TBias = value1 - value2;
		Turn_Pwm = Tkp *  TBias - Tkd * (TBias - Last_TBias);
	   Speed_Pwm1 = Speed_Pwm1 - Turn_Pwm;
       Speed_Pwm2 = Speed_Pwm2 + Turn_Pwm;
}

	 
//如果value1 < value2,说明车身向左偏转，此时左轮加速右轮减速
else
{
		TBias = value2 - value1;
		Turn_Pwm = Tkp *  TBias;// - Tkd * (TBias - Last_TBias);
	  Speed_Pwm1 = Speed_Pwm1 + Turn_Pwm;
      Speed_Pwm2 = Speed_Pwm2 - Turn_Pwm;
}

  Last_TBias = TBias;
	
  Speed_xianfu();
	
  PWM_Set(Speed_Pwm1,Speed_Pwm2);
}
