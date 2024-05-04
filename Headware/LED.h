#ifndef _LED_H
#define _LED_H

void LED_Init(void);
void LED_PWM_Init(void);
void LED_ON(void);
void LED_OFF(void);
void LED_Turn(void);
void LED1_PWM(uint8_t ms);
void LED2_PWM(uint8_t ms);
void LED3_PWM(uint8_t ms);

#endif
