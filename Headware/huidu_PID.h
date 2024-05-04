#ifndef _HUIDU_PID_H
#define _HUIDU_PID_H

extern int16_t huidu[3];
extern int Speed_Pwm1,Speed_Pwm2,div_Pwm,Speed1,Speed2;
extern int Turn_Pwm,TBias,sum;


void huidu_Init(void);
int huidu_detect(void);


int PositionPID(int16_t Value);

void Turn_PID(int16_t value1,int16_t value2);

void Speed_PID(int Speed_exp);
#endif
