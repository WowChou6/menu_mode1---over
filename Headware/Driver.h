#ifndef _DRIVER_H
#define _DRIVER_H

void Driver_Init(void);

void D_GO(uint16_t spead);
void D_Stop(void);
void D_TurnRight(uint16_t spead);
void D_TurnLeft(uint16_t spead);

uint8_t Get_Spead(void);
uint8_t Get_DetectValue(void);

#endif
