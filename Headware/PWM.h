#ifndef _PWM_H
#define _PWM_H

extern uint8_t num;

void PWM_Init(uint32_t Freq);
void PWM_SetCompare1(uint8_t Compare);
void PWM_SetCompare2(uint8_t Compare);
void PWM_SetCompare3(uint8_t Compare);
void PWM_Stepping(void);
void IC_Init(void);
uint32_t IC_GetFreq(void);
uint32_t IC_GetDuty(void);
#endif
