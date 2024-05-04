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

int L_pos=0,R_pos=0,L_vel,R_vel;  //������λ�� �ٶ�
int xunji_different_vel = 0;      //ѭ���������ٶȲ�
int base_vel = 20;                //�����ٶ�'
int pos;                          //С��ƫ��������λ��
int huidu_data;                   //�Ҷȴ������������
int stage_count;                  
int stage = 3;    
float Pitch,Roll,Yaw;
int mode2_dest;
int mode1_dest = 0;
int finalYaw = 999;
int initialYaw = 999;
int turnFinishFlag = 0;

int menu2 = 0;     //���嵱ǰģʽ

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
////        L_vel = Encoder1_Get();  //��ȡ��λʱ�����ֱ�������ֵ���������㣨�������ٶȣ�
////		R_vel = Encoder2_Get();             //

//       huidu_data = huidu_detect();  //�õ��Ҷȱ�������   
//      pos = xunji(huidu_data);      //�õ�С��λ��ƫ����
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


void PIDSpeed(int current_speed) {
	set_L_pwm(Velocity_L(L_vel, current_speed - xunji_different_vel));
	set_R_pwm(Velocity_R(R_vel, current_speed + xunji_different_vel));
}

int moveUntilCross(int current_speed) {
	PIDSpeed(current_speed);
	if (pos == 100)
	{ 
		L_pos = R_pos = 0;
		return 1;
	} return 0;
}

int moveUntilCrossAndMinDistance(int current_speed, int min_detect_distance) {
	PIDSpeed(current_speed);
	if ((L_pos + R_pos) / 2 > min_detect_distance && pos == 100)
	{ 
		L_pos = R_pos = 0;
		return 1;
	} return 0;
}

int moveOverflow(int overflow_distance){
	set_L_pwm(Velocity_L(L_vel, base_vel));
	set_R_pwm(Velocity_R(R_vel, base_vel));
	if ((L_pos + R_pos) / 2 > overflow_distance)
	{
		L_pos = R_pos = 0;
		brake();
		return 1;
	} return 0;
}



int turn(int routate) {
	if(finalYaw == 999){
		initialYaw = Yaw;
		finalYaw = Yaw + routate;
	}

	int currentYaw = Yaw - initialYaw;

	if (turnFinishFlag == 0)
	{
		set_L_pwm(-MTurn(currentYaw,finalYaw));
		set_R_pwm(MTurn(currentYaw,finalYaw));
	}
	
	
	if (abs(MTurn(currentYaw,finalYaw))  <= 5000) {
		turnFinishFlag = 1;
	}

	if(turnFinishFlag)
	{
		set_L_pwm(Velocity_L(L_vel,-5));
		set_R_pwm(Velocity_R(R_vel, 5));	
	}
	else {
		set_L_pwm(Velocity_L(L_vel, 5));
		set_R_pwm(Velocity_R(R_vel, -5));

	}

   if(huidu_data == 0x80 || huidu_data == 0x20) //01000 00010
   {
		stage_count = 0;
		L_pos = R_pos = 0;
		initialYaw = 999;
		finalYaw = 999;
		brake();
		return 1;
	}
	return 0;
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
	   
        L_vel = Encoder1_Get();  //��ȡ��λʱ�����ֱ�������ֵ���������㣨�������ٶȣ�
		R_vel = Encoder2_Get();  //
		R_pos += R_vel;               //�����ٶ��ۼ�Ϊλ��
		L_pos += L_vel;               //
        
	    
	    MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);
        huidu_data = huidu_detect();  //�õ��Ҷȱ�������   
        pos = xunji(huidu_data);      //�õ�С��λ��ƫ����
     if (pos != 100) xunji_different_vel = pos;  //pos=100������������ڣ���Ϊ100��õ�ѭ�����������ٶȲ�
        //OLED_ShowSignedNum(1,1,Yaw,5);
//  //  if(menu2 == 0)
//	//{
         switch (stage)
			{
			 		
			case 0:
				   Speak();
			       stage = 1;
			      break;
			
			case 1:  //����·��
				if(moveUntilCross(20)){
					stage = 2;
				}
				break;
				
			case 2:  //����ֱ�� ֱ�� �����ߵ������
				if(moveOverflow(100)){
					stage = 3;
				}
				break;
				
			case 3:  //��תֱ��
				stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //�ȴ�0.1s�ٿ�ʼת��


				if (stage_count > 10)
				{
					if(turn(-90)){
						stage = 200;
					}
		     	}           
				
        
				break;
				
			case 4:  //����ֱ�� ��������⵽�����
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 700)  //���ｻ���
				{
					L_pos = R_pos = 0;
					stage = 5;
					brake();
					
				} break;
				
		   	case 5:  //����ֱ�� �����ߵ������
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 100)
				{
					L_pos = R_pos = 0;
					brake();
					stage = 6;
				} break;
				
			case 6:  //��תֱ��
				stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //�ȴ�0.1s�ٿ�ʼת��
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
				
			case 7:   //����ֱ�� ֱ�� ��������⵽�����
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 3500 && pos == 100)
				{
					L_pos = R_pos = 0;
					brake();
			
					stage = 9;
				}break;
//				
			case 8:  //����ֱ�� ֱ�� �����ߵ������
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
			case 9:  //��תֱ��
				stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //�ȴ�0.1s�ٿ�ʼת��
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
				
				
				case -1:  //��תֱ��
				 
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 50)
				{
					L_pos = R_pos = 0;
					brake();
					stage = 10;
				
				} break;;
				
			case 10:  //����ֱ�� ֱ�� ��������⵽�����
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 2500 && pos == 100)  /**/
				{
					L_pos = R_pos = 0;
					brake();
					stage = 12;
				} break;
				
//			case 11:  //����ֱ�� ֱ�� �����ߵ������
//				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
//				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
//				if ((L_pos + R_pos) / 2 > 50)
//				{
//					L_pos = R_pos = 0;
//					brake();
//					stage = 12;
//				
//				} break;
       
            case 12:  //��תֱ��
                stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //�ȴ�0.1s�ٿ�ʼת��
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
				
		    case 13:  //����ֱ�� ֱ�� ��������⵽����� 
				
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 3500 && pos == 100)
				{
					L_pos = R_pos = 0;
					stage = 14;
				}break;
			case 14:  //����ֱ�� ֱ�� �����ߵ������
		        set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 250)
				{
					L_pos = R_pos = 0;
					brake();
					stage = 15;
				} break;
				
			case 15: //��תֱ��
				 stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //�ȴ�0.1s�ٿ�ʼת��
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
				
			case 16: //����ֱ�� ֱ�� ��������⵽�����
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 500&& pos == 100)
				{
					brake();
					L_pos = R_pos = 0;
					stage = 18;
				}break;
				
			case 17:  //����ֱ�� ֱ�� �����ߵ������
		        set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
				if ((L_pos + R_pos) / 2 > 100)
				{
					L_pos = R_pos = 0;
					brake();
					stage = 18;
				} break;	
//			
     		 case 18:  //��תֱ��
               stage_count++;
				if (stage_count == 10) L_pos = R_pos = 0;  //�ȴ�0.1s�ٿ�ʼת��
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
             
               case 19:  //����ֱ�� ֱ�� ��������⵽�յ�
				set_L_pwm(Velocity_L(L_vel, base_vel - xunji_different_vel));
				set_R_pwm(Velocity_R(R_vel, base_vel + xunji_different_vel));
			
				if ((L_pos + R_pos) / 2 > 300 && pos == 100)
				{
					L_pos = R_pos = 0;
					stage = 20;
					brake();
				} break;	
               
			   case 20:  //ͣ��
	
					L_pos = R_pos = 0;
					brake();
					stage = 21;
				     break;
			     case 21: 
			     Speak(); //������������
                 stage = 22;//����
				 break; 	
			}
//			
// 	}
// }
 }
}

