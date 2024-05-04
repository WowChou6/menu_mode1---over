 #include "stm32f10x.h"
#include "I2C.h"
#include "Delay.h"


void LCD_Send(uint8_t data)
{
   I2C_Send(data);
   I2C_ReceiveACK();
}

void LCD_EN(uint8_t data)
{ 
   data |= 0x04;
  LCD_Send(data);
   data &= 0xFB;
  LCD_Send(data);
	
}

void LCD_Cmd(uint8_t cmd)
{    
	 
	  
      uint8_t LCD_data = 0x08;
	
	
      LCD_data &= 0X0f; 
      LCD_data |= (cmd & 0xf0);
	
      LCD_Send(LCD_data);
	
      LCD_EN(LCD_data);
      cmd = cmd<<4;
      LCD_data &= 0X0f; 
      LCD_data |= cmd & 0xf0; 
      LCD_Send(LCD_data);

      LCD_EN(LCD_data);
	
	// I2C_Stop();
	
}

void LCD_Data(uint8_t dat)

{     //I2C_Start();
	 // LCD_Send(0x4E);
	 
      uint8_t LCD_data;
      LCD_data = 0x09;
	
      LCD_data &= 0X0f; 
      LCD_data |= dat & 0xf0;
      LCD_Send(LCD_data);
      LCD_EN(LCD_data);
	
      dat = dat<<4;
      LCD_data &= 0X0f;
      LCD_data |= dat & 0xf0; 
      LCD_Send(LCD_data);
       LCD_EN(LCD_data);
 
}

void LCD_Init(void)
{    
	I2C_INIT();
	 I2C_Start();
	LCD_Send(0x4E);
	LCD_Cmd(0x28);
	LCD_Cmd(0X06);
	LCD_Cmd(0X0f);
	LCD_Cmd(0x01);
	
	LCD_Cmd(0x80);
	LCD_Data(0X61);
}



                            




