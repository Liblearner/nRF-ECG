//#include "sc7a20.h"

///*************************************************************************************************************
// * �ļ���:			SC7A20_Test.c
// * ����:			SC7A20����
// * ����:			cp1300@139.com
// * ����ʱ��:		2022-07-01
// * ����޸�ʱ��:	2022-07-01
// * ��ϸ:			
//*************************************************************************************************************/
// 
//��ȡ�Ĵ����ӿ�
//static bool SC7A20_IIC_ReadReg(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *pDataBuff, uint16_t ByteNum)
//{
//	if (IIC_MasterReadReg(SC7A20_IIC_CH, SlaveAddr, RegAddr, 1, pDataBuff, ByteNum) != IIC_OK) return 0;
//	else return 1;
//}
//	
// 
////д�Ĵ����ӿ�
//static bool SC7A20_IIC_WriteReg(uint16_t SlaveAddr, uint8_t RegAddr, uint8_t *pDataBuff, uint16_t ByteNum)
//{
//	if (IIC_MasterWriteReg(SC7A20_IIC_CH, SlaveAddr, RegAddr, 1, pDataBuff, ByteNum) != IIC_OK) return 0;
//	else return 1;
//}
// 
////SC7A20����
//void SC7A20_Test(void)
//{
//	uint8_t buff[7];
//	short acc[3];
//	float zAngle;
//	
//	IIC_Init(SC7A20_IIC_CH, 300, 100);//Ӳ��IIC��ʼ��
// 
//	//SC7A20��ʼ��
//	SC7A20_Init(&sg_Temp_SC7A20_Handle, SC7A20_ADDR, SC7A20_IIC_ReadReg, SC7A20_IIC_WriteReg);
// 
//	while (1)
//	{
//		if (0 == SC7A20_GetZAxisAngle(&sg_Temp_SC7A20_Handle, acc, &zAngle))//SC7A20��ȡ���ٶ�ֵ
//		{
//			INFO_S("���ٶ����ݶ�ȡʧ��\r\n");
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
