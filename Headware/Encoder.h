#ifndef _ENCODER_H
#define _ENCODER_H


void Encoder_Init(void);

int16_t Encoder1_Get(void);
int16_t Encoder2_Get(void);

void PWM_Set(int16_t PWM1,int16_t PWM2);

void set_L_pwm(int PWM1);
void set_R_pwm(int PWM2);

int xunji(int huidu_data);

void Timer_Init(void);

int Velocity_L(int L_vel, int L_vel_target);
int Velocity_R(int R_vel, int R_vel_target);
int MTurn(int turn_pos, int turn_target);
int Turn(int turn_pos, int turn_target);

void brake(void);

#endif
