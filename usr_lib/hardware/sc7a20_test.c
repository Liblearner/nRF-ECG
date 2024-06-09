//#include "sc7a20.h"

///*************************************************************************************************************
// * 文件名:			SC7A20_Test.c
// * 功能:			SC7A20测试
// * 作者:			cp1300@139.com
// * 创建时间:		2022-07-01
// * 最后修改时间:	2022-07-01
// * 详细:			
//*************************************************************************************************************/
// 
//读取寄存器接口
//static bool SC7A20_IIC_ReadReg(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *pDataBuff, uint16_t ByteNum)
//{
//	if (IIC_MasterReadReg(SC7A20_IIC_CH, SlaveAddr, RegAddr, 1, pDataBuff, ByteNum) != IIC_OK) return 0;
//	else return 1;
//}
//	
// 
////写寄存器接口
//static bool SC7A20_IIC_WriteReg(uint16_t SlaveAddr, uint8_t RegAddr, uint8_t *pDataBuff, uint16_t ByteNum)
//{
//	if (IIC_MasterWriteReg(SC7A20_IIC_CH, SlaveAddr, RegAddr, 1, pDataBuff, ByteNum) != IIC_OK) return 0;
//	else return 1;
//}
// 
////SC7A20测试
//void SC7A20_Test(void)
//{
//	uint8_t buff[7];
//	short acc[3];
//	float zAngle;
//	
//	IIC_Init(SC7A20_IIC_CH, 300, 100);//硬件IIC初始化
// 
//	//SC7A20初始化
//	SC7A20_Init(&sg_Temp_SC7A20_Handle, SC7A20_ADDR, SC7A20_IIC_ReadReg, SC7A20_IIC_WriteReg);
// 
//	while (1)
//	{
//		if (0 == SC7A20_GetZAxisAngle(&sg_Temp_SC7A20_Handle, acc, &zAngle))//SC7A20读取加速度值
//		{
//			INFO_S("加速度数据读取失败\r\n");
//		}
//		else
//		{
//			INFO_S("Z:%f\r\n", zAngle);
//		}
// 
//		SYS_DelayMS(500);
//	}
// 
//}	
// 
