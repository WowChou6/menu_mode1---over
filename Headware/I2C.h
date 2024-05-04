#ifndef _I2C_H
#define _I2C_H
void I2C_INIT(void);
void I2C_SCL(uint8_t Bit);
void I2C_SDA(uint8_t Bit);
uint8_t I2C_SDA_R(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendBit(uint8_t Bit);
void I2C_Send(uint8_t Byte);
uint8_t I2C_Read(void);
void I2C_SendACK(uint8_t ACKBit);
uint8_t I2C_ReceiveACK(void);


#endif
