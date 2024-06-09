#include "sc7a20.h"

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(0);

const nrf_drv_twi_config_t twi_sc7a20_config = {
 .scl                = 23,
 .sda                = 22,
 .frequency          = NRF_DRV_TWI_FREQ_100K,
 .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
 .clear_bus_init     = false
};

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
switch(p_event->type)
{
	case 
		NRF_DRV_TWI_EVT_DONE:
	{m_xfer_done = true;
		break;} 
	default:
		break;
}
}
	
//SC7A20���ӿ�
bool SC7A20_IIC_ReadReg(uint8_t dev_id, uint8_t address, uint8_t *pDataBuff, uint8_t length)
{	
	ret_code_t err_code;
	m_xfer_done = false;
	//����SC7A20��ַ����Ѱַ��true��ʾд�겻����ֹͣ��־
	err_code = nrf_drv_twi_tx(&m_twi, SC7A20_Address, &address,1,false);
	while(m_xfer_done == false)
	{}
	if(err_code != NRF_SUCCESS)
	{
	return false;
	}
	m_xfer_done = false;
	err_code = nrf_drv_twi_rx(&m_twi, SC7A20_Address, pDataBuff, length);
	while(m_xfer_done == false)
	{}
	if(err_code != NRF_SUCCESS)
	{
	return false;
	}
	return true;
}
	
 
//SC7A20дһ���ֽ�
bool SC7A20_IIC_WriteReg(uint8_t dev_id, uint8_t address, uint8_t *pDataBuff, uint8_t length)
{
	ret_code_t err_code;
	uint8_t write_data[2];
	write_data[0] = address;
	memcpy(&write_data[1], pDataBuff, length);
	//TWI������ɱ�־����Ϊfalse
	m_xfer_done = false;
	err_code = nrf_drv_twi_tx(&m_twi,dev_id,write_data, length+1, false);
	while(m_xfer_done == false){}
	if(m_xfer_done != err_code)
	{
	return false;
	}
	return true;
}
	
/*************************************************************************************************************************
*����        	:	bool SC7A20_Init(SC7A20_HANDLE *pHandle, uint8_t SlaveAddr, 
						bool (*IIC_ReadReg)(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *pDataBuff, uint16_t ByteNum), 
						bool (*IIC_WriteReg)(uint16_t SlaveAddr, uint8_t RegAddr, uint8_t *pDataBuff, uint16_t ByteNum))
*����        	:	SC7A20��ʼ��
*����        	:	pHandle:�����SlaveAddr��оƬIICд��ַ��IIC_ReadReg��IIC��ȡ�ӿڣ�IIC_WriteReg:IICд��ӿڣ�
*����        	:	1:��ʼ���ɹ���0:��ʼ��ʧ��
*����			: 	�ײ�궨��
*����       		:	cp1300@139.com
*ʱ��     		:	2022-07-01
*����޸�ʱ��		:	2022-07-01
*˵��        	:	
*************************************************************************************************************************/
bool SC7A20_Init(void)
{
	uint8_t tempreg = 0;

  nrf_gpio_pin_write(SC7A20_CS, 1);
	nrf_gpio_pin_write(SC7A20_SDA, 1);
	nrf_gpio_pin_write(SC7A20_SCL, 1);

	//nrf��twi��ʼ��
	ret_code_t err_code;
	err_code = nrf_drv_twi_init(&m_twi, &twi_sc7a20_config, twi_handler, NULL);
	APP_ERROR_CHECK(err_code);
	nrf_drv_twi_enable(&m_twi);
 
	nrf_delay_ms(100);
	
	//��WHO_AM_I
	read_buffer[0] = 0;
	init_state = SC7A20_IIC_ReadReg(SC7A20_Address, SC7A20_REG_WHO_AM_I, &read_buffer[0], 1);
	if(read_buffer[0] == 0x11)
		init_state = 1;
	else
		init_state = 0;
	nrf_delay_ms(100);


	tempreg = 0x47;
	SC7A20_IIC_WriteReg(SC7A20_Address, SC7A20_REG_CTRL_1, &tempreg, 1);//50Hz+����ģʽxyzʹ��
	tempreg = 0x00;
	SC7A20_IIC_WriteReg(SC7A20_Address, SC7A20_REG_CTRL_2, &tempreg, 1);//�ر��˲������ֲ�����û���˲�����ֹƵ������˵�����������޷�������ֹ״̬�µ��������ٶ�
	tempreg = 0x00;
	SC7A20_IIC_WriteReg(SC7A20_Address, SC7A20_REG_CTRL_3, &tempreg, 1); //�ر��ж�
	tempreg = 0x88;
	SC7A20_IIC_WriteReg(SC7A20_Address, SC7A20_REG_CTRL_4, &tempreg, 1); //��ȡ����ٸ��£�С��ģʽ����2g+����ģʽ���߾���ģʽ

	return 0;
}
 
short SC7A20_12bitComplement(uint8_t msb, uint8_t lsb)
{
	short temp;
 
	temp = msb << 8 | lsb;
	temp = temp >> 4;   //ֻ�и�12λ��Ч
	temp = temp & 0x0fff;
	if (temp & 0x0800) //���� ����==>ԭ��
	{
		temp = temp & 0x07ff; //�������λ      
		temp = ~temp;
		temp = temp + 1;
		temp = temp & 0x07ff;
		temp = -temp;       //��ԭ���λ
	}
	return temp;
}
 
 
/*************************************************************************************************************************
*����        	:	bool SC7A20_ReadAcceleration(SC7A20_HANDLE* pHandle, short* pXa, short* pYa, short* pZa)
*����        	:	SC7A20��ȡ���ٶ�ֵ
*����        	:	pHandle:�����pXa��pYa,pZa ������ٶ�ֵ
*����        	:	1:�ɹ���0:ʧ��
*����			: 	�ײ�궨��
*����       		:	cp1300@139.com
*ʱ��     		:	2022-07-01
*����޸�ʱ��		:	2022-07-01
*˵��        	:
*************************************************************************************************************************/
bool SC7A20_ReadAcceleration(short* pXa, short* pYa, short* pZa)
{
	uint8_t buff[6] = {0,0,0,0,0,0};
//	uint8_t i;
 
	memset(buff, 0, 6);

	SC7A20_IIC_ReadReg(SC7A20_Address, SC7A20_REG_X_L|0x80, buff, 6);

	//X��
	*pXa = buff[1];
	*pXa <<= 8;
	*pXa |= buff[0];
	*pXa >>= 4;	//ȡ12bit����

	//Y��
	*pYa = buff[3];
	*pYa <<= 8;
	*pYa |= buff[2];
	*pYa >>= 4;	//ȡ12bit����
	
	//Z��
	*pZa = buff[5];
	*pZa <<= 8;
	*pZa |= buff[4];
	*pZa >>= 4;	//ȡ12bit����

	return 1;
 
}
 
#define PI 3.1415926535898
/*************************************************************************************************************************
*����        	:	bool SC7A20_GetZAxisAngle(SC7A20_HANDLE* pHandle, short AcceBuff[3], float* pAngleZ)
*����        	:	SC7A20 ��ȡZ�����
*����        	:	pHandle:�����AcceBuff:3����ļ��ٶȣ�pAngleZ��Z�������
*����        	:	1:�ɹ���0:ʧ��
*����			: 	�ײ�궨��
*����       		:	cp1300@139.com
*ʱ��     		:	2022-07-02
*����޸�ʱ��		:	2022-07-02
*˵��        	:
*************************************************************************************************************************/
bool SC7A20_GetZAxisAngle(short AcceBuff[3], float* pAngleZ)
{
	double fx, fy, fz;
	double A;
	short Xa, Ya, Za;
  SC7A20_ReadAcceleration(&Xa, &Ya, &Za);
	//uart_printf("Xa:%d \tYa:%d \tZa:%d \r\n",Xa,Ya,Za);
	AcceBuff[0] = Xa;	//x����ٶ�
	AcceBuff[1] = Ya;	//y����ٶ�
	AcceBuff[2] = Za;	//z����ٶ�
 
	fx = Xa;
	fx *= 2.0 / 4096;
	fy = Ya;
	fy *= 2.0 / 4096;
	fz = Za;
	fz *= 2.0 / 4096;
 
	//uart_printf("fx��%.04f\tfy��%.04f\tfz��%.04f\t\r\n",fx,fy,fz);
 
	//Z����
	A = fx * fx + fy * fy;
	A = sqrt(A);
	A = (double)A / fz;
	A = atan(A);
	A = A * 180 / PI;
 
	*pAngleZ = A;
	//uart_printf("=======�Ƕȣ�%.04f\r\n",*pAngleZ);
 
 
	return 1;
}

