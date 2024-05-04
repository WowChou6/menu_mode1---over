#include "stm32f10x.h"
#include "Delay.h"

#define SCL  GPIO_Pin_12
#define SDA  GPIO_Pin_13


void I2C_INIT(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
   GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = SCL|SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
   GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOB,SDA);
   	GPIO_SetBits(GPIOB,SCL);
	
}

void I2C_SCL(uint8_t Bit)
{
 GPIO_WriteBit(GPIOB,SCL,(BitAction)Bit);
 Delay_ms(5); 
}

void I2C_SDA(uint8_t Bit)
{
  GPIO_WriteBit(GPIOB,SDA,(BitAction)(Bit));
  Delay_ms(5);
}

uint8_t I2C_SDA_R(void)
{
    uint8_t Bitvalue;
    Bitvalue = GPIO_ReadInputDataBit(GPIOB,SDA);
   	Delay_ms(5);
	return Bitvalue;
}

void I2C_Start(void)
{
    I2C_SDA(1);
	I2C_SCL(1);
	I2C_SDA(0);
	I2C_SCL(0);
}

void I2C_Stop(void)
{
   I2C_SDA(0);
   I2C_SCL(1);
   I2C_SDA(1);
}

void I2C_SendBit(uint8_t Bit)
{
 if(Bit){
          I2C_SDA(1);
 }
 else I2C_SDA(0);
   I2C_SCL(1);
   I2C_SCL(0); 
}

void I2C_Send(uint8_t Byte)
{
  for(uint8_t i = 0;i <= 7;i++)
	{
	 I2C_SendBit(Byte & (0x80>>i));
	}
}

uint8_t I2C_Read(void)
{
	uint8_t Byte = 0x00;
    I2C_SDA(1);
	
    for(uint8_t i = 0; i <= 7;i++)
	{
		I2C_SCL(1);
	   if(I2C_SDA_R() == 1) Byte|=(0x80 >> i);
		I2C_SCL(0);
	}
  
	return Byte;
}

void I2C_SendACK(uint8_t ACKBit)
{
  I2C_SDA(ACKBit);
  I2C_SCL(1);
  I2C_SCL(0);
}

uint8_t I2C_ReceiveACK(void)
{
  uint8_t ACKBit = 0x00;
  I2C_SDA(1);
  I2C_SCL(1);
  ACKBit = I2C_SDA_R();
  I2C_SCL(0);
	return ACKBit;
}



	

