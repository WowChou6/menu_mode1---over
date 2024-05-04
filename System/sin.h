#ifndef _SIN_H
#define _SIN_H

void Set_Sine12bit(float MAX,float MIN);
void Set_Period(uint32_t value);
void TIM2_Int_Init(uint32_t Hz);
void DAC_DMA_Config(void);

#endif
