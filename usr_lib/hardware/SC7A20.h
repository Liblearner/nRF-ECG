#ifndef SC7A20_H
#define SC7A20_H

#include "nrf_delay.h"
#include "main.h"
/*
SC7A20：

SC7A20_CS:			P0.24：0为SPI、1为IIC
SC7A20_SDO:			P0.21：SPI四线SDO
SC7A20_SDI:			P0.22：IIC下做SDA；SPI三线SDA，SPI四线SDI
SC7A20_SCK:			P0.23：IIC与SPI下均为时钟SCK

SC7A20_INT1:		P0.25：用户编程中断
SC7A20_INT2:		P1.00：用户编程中断
*/

#define SC7A20_IIC

//IO定义
#ifdef SC7A20_IIC

#define SC7A20_CS          	NRF_GPIO_PIN_MAP(0,24)
#define SC7A20_SDA          NRF_GPIO_PIN_MAP(0,22)
#define SC7A20_SCL          NRF_GPIO_PIN_MAP(0,23)
#define SC7A20_INT1         NRF_GPIO_PIN_MAP(0,25)
#define SC7A20_INT2         NRF_GPIO_PIN_MAP(1,0)

#endif

#ifdef SC7A20_SPI3

#define SC7A20_CS          	NRF_GPIO_PIN_MAP(0,3)
#define SC7A20_SDO          NRF_GPIO_PIN_MAP(1,15)
#define SC7A20_SDI          NRF_GPIO_PIN_MAP(1,14)
#endif

#ifdef SC7A20_SPI4

#define SC7A20_CS          	NRF_GPIO_PIN_MAP(0,3)
#define SC7A20_SDO          NRF_GPIO_PIN_MAP(1,15)
#define SC7A20_SDI          NRF_GPIO_PIN_MAP(1,14)
#endif

#define TWI_INSTANCE_ID     		0

#define SC7A20_Address 					0x18

#define SC7A20_REG_WHO_AM_I     0x0f
#define SC7A20_REG_CTRL_1				0x20
#define SC7A20_REG_CTRL_2				0x21
#define SC7A20_REG_CTRL_3				0x22
#define SC7A20_REG_CTRL_4				0x23
#define SC7A20_REG_X_L          0x28
#define SC7A20_REG_X_H          0x29
#define SC7A20_REG_Y_L          0x2A
#define SC7A20_REG_Y_H          0x2B
#define SC7A20_REG_Z_L          0x2C
#define SC7A20_REG_Z_H          0x2D
#define SC7A20_REG_STATUS				0x27

 
bool SC7A20_IIC_ReadReg(uint8_t dev_id, uint8_t address, uint8_t *pDataBuff, uint8_t length);
bool SC7A20_IIC_WriteReg(uint8_t dev_id, uint8_t address, uint8_t *pDataBuff, uint8_t length);
bool SC7A20_Init(void);
bool SC7A20_ReadAcceleration(short* pXa, short* pYa, short* pZa);//SC7A20读取加速度值
bool SC7A20_GetZAxisAngle(short AcceBuff[3], float* pAngleZ);//SC7A20 获取Z轴倾角
 
#endif /*_SC7A20_H_*/


