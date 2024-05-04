#include "stm32f10x.h"  
#include "OLED.h"
#include "Delay.h"
#include "menu.h"
#include "Key.h"
#include "huidu_PID.h"
#include "Encoder.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "Speaker.h"

//Device header

int L_pos=0,R_pos=0,L_vel,R_vel;  //左右轮位移 速度
int xunji_different_vel = 0;      //循迹左右轮速度差
int base_vel = 20;                //基础速度'
int pos;                          //小车偏离中心线位移
int huidu_data;                   //灰度传感器输出编码
int stage_count;                  
int stage = 3;    
float Pitch,Roll,Yaw;
int mode2_dest;
int mode1_dest = 0;

int menu2 = 0;     //定义当前模式

int main(void){	
    Speaker_Init();
	OLED_Init();
	Encoder_Init();
    huidu_Init();
    MPU6050_Init();
	MPU6050_DMP_Init();
    Timer_Init();

while(1)
{  
////	
//       MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);
////        L_vel = Encoder1_Get();  //读取单位时间右轮编码器数值，读完清零（即右轮速度）
////		R_vel = Encoder2_Get();             //

//       huidu_data = huidu_detect();  //得到灰度编码数据   
//      pos = xunji(huidu_data);      //得到小车位置偏移量
//////////////	

////////	   OLED_ShowSignedNum(3,1,Yaw,5);
//	  OLED_ShowBinNum(1,1,huidu_data,5);
////////	
//////	    OLED_ShowSignedNum(1,1,Yaw,5);

////	  set_L_pwm(Velocity_L(L_vel, base_vel));
////	  set_R_pwm(Velocity_R(R_vel, base_vel));
//////      if (pos != 100) xunji_different_vel = xunji(huidu_data);
////	    
////        
////	   OLED_ShowSignedNum(1,1,L_vel,5);
////	   OLED_ShowSignedNum(2,1,R_vel,5); 
////////	brake();	
//        Delay_ms(10);
//	  
}

}


//    menu2 = menu1();
//	if(menu2 == 1) mode1();
//	if(menu2 == 2)  mode2_dest = mode2();
//	if(menu2 == 3) mode3();
//	if(menu2 == 4) mode4();
void TIM1_UP_IRQHandler()
{
   if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
   {
	   TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	   
        L_vel = Encoder1_Get();  //读取单位时间右轮编码器数值，读完清零（即左轮速度）
		R_vel = Encoder2_Get();  //
		R_pos += R_vel;               //右轮速度累加为位移
		L_pos += L_vel;               //
        
	    
	    MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);
        huidu_data = huidu_detect();  //得到灰度编码数据   
        pos = xunji(huidu_data);      //得到小车位置偏移量
     if (pos != 100) xunji_different_vel = pos;  //pos=100代表遇到交叉口，不为100则得到循迹的左右轮速度差
        //OLED_ShowSignedNum(1,1,Yaw,5);
//  //  if(menu2 == 0)
//	//{
         switch (stage)
			{
			 		
			case 0:
				   Speak();
			       stage = 1;
			      break;
			
			case 1:  //干线直行 直到 传感器检测到交叉口
			// asddsa
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
			
				if ((L_pos + R_pos) / 2 > 2500 && pos == 100)
				{ 
					L_pos = R_pos = 0;
					stage = 2;
				} break;
				
			case 2:  //干线直行 直到 轮子走到交叉口
				set_L_pwm(Velocity_L(L_vel, base_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel));
				if ((L_pos + R_pos) / 2 > 100)
				{
					L_pos = R_pos = 0;
					brake();
					stage = 3;
				} break;
				
				
			case 3:  //左转直角
				stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //等待0.1s再开始转向
				if (stage_count > 10)
				{
						set_L_pwm(-MTurn(Yaw,90));
						set_R_pwm(MTurn(Yaw,90));
		 
   
		     	}           
				if (Yaw >= 90) 
				{
					stage_count = 0;
					L_pos = R_pos = 0;
					brake();
					stage = 200;
				}
        
				break;
				
			case 4:  //干线直行 传感器检测到交叉口
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 700)  //到达交叉口
				{
					L_pos = R_pos = 0;
					stage = 5;
					brake();
					
				} break;
				
		   	case 5:  //干线直行 轮子走到交叉口
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 100)
				{
					L_pos = R_pos = 0;
					brake();
					stage = 6;
				} break;
				
			case 6:  //右转直角
				stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //等待0.1s再开始转向
				if (stage_count > 10)
				{
						set_L_pwm(-MTurn(Yaw,0));
						set_R_pwm(MTurn(Yaw,0));
					
				}
				if (Yaw <= 5) 
				{
					stage_count = 0;
					L_pos = R_pos = 0;
					brake();
					stage = 7;
				} break;
				
			case 7:   //干线直行 直到 传感器检测到交叉口
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 3500 && pos == 100)
				{
					L_pos = R_pos = 0;
					brake();
			
					stage = 9;
				}break;
//				
			case 8:  //干线直行 直到 轮子走到交叉口
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 50)
				{
					brake();
					//stage_count++;
					//if (stage_count >= 100)
					
						//stage_count = 0;
						L_pos = R_pos = 0;
						stage = 9;
					
				} break;
//				
			case 9:  //左转直角
				stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //等待0.1s再开始转向
				if (stage_count > 10)
				{
						set_L_pwm(-MTurn(Yaw,85));
						set_R_pwm(MTurn(Yaw,85));
					
				}
				if (Yaw >= 80) 
				{
					stage_count = 0;
					L_pos = R_pos = 0;
					brake();
					stage = -1;
				} break;
				
				
				case -1:  //左转直角
				 
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 50)
				{
					L_pos = R_pos = 0;
					brake();
					stage = 10;
				
				} break;;
				
			case 10:  //干线直行 直到 传感器检测到交叉口
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 2500 && pos == 100)  /**/
				{
					L_pos = R_pos = 0;
					brake();
					stage = 12;
				} break;
				
//			case 11:  //干线直行 直到 轮子走到交叉口
//				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
//				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
//				if ((L_pos + R_pos) / 2 > 50)
//				{
//					L_pos = R_pos = 0;
//					brake();
//					stage = 12;
//				
//				} break;
       
            case 12:  //左转直角
                stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //等待0.1s再开始转向
				if (stage_count > 10)
				{
						set_L_pwm(-MTurn(Yaw,179));
						set_R_pwm(MTurn(Yaw,179));
					
				}
				if (Yaw >= 179) 
				{
					stage_count = 0;
					L_pos = R_pos = 0;
					brake();
					stage = 13;
					
				} break;
				
		    case 13:  //干线直行 直到 传感器检测到交叉口 
				
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 3500 && pos == 100)
				{
					L_pos = R_pos = 0;
					stage = 14;
				}break;
			case 14:  //干线直行 直到 轮子走到交叉口
		        set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 250)
				{
					L_pos = R_pos = 0;
					brake();
					stage = 15;
				} break;
				
			case 15: //右转直角
				 stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //等待0.1s再开始转向
				if (stage_count > 10)
				{
						set_L_pwm(-MTurn(Yaw,90));
						set_R_pwm(MTurn(Yaw,90));
					
				}
				if (Yaw <= 90) 
				{
					stage_count = 0;
					L_pos = R_pos = 0;
					brake();
					stage = 16;
				} break;
				
			case 16: //干线直行 直到 传感器检测到交叉口
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 500&& pos == 100)
				{
					brake();
					L_pos = R_pos = 0;
					stage = 18;
				}break;
				
			case 17:  //干线直行 直到 轮子走到交叉口
		        set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 100)
				{
					L_pos = R_pos = 0;
					brake();
					stage = 18;
				} break;	
//			
     		 case 18:  //左转直角
               stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //等待0.1s再开始转向
				if (stage_count > 10)
				{
						set_L_pwm(-MTurn(Yaw,175));
						set_R_pwm(MTurn(Yaw,175));
					
				}
				if (Yaw >= 175) 
				{
					stage_count = 0;
					L_pos = R_pos = 0;
					brake();
					stage = 19;
					
				} break;	
             
               case 19:  //干线直行 直到 传感器检测到终点
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
			
				if ((L_pos + R_pos) / 2 > 300 && pos == 100)
				{
					L_pos = R_pos = 0;
					stage = 20;
					brake();
				} break;	
               
			   case 20:  //停下
	
					L_pos = R_pos = 0;
					brake();
					stage = 21;
				     break;
			     case 21: 
			     Speak(); //蜂鸣器响三声
                 stage = 22;//待机
				 break; 	
			}
//			
// 	}
// }
 }
}

