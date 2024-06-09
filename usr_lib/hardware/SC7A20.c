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
	
//SC7A20读接口
bool SC7A20_IIC_ReadReg(uint8_t dev_id, uint8_t address, uint8_t *pDataBuff, uint8_t length)
{	
	ret_code_t err_code;
	m_xfer_done = false;
	//发出SC7A20地址进行寻址，true表示写完不发送停止标志
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
	
 
//SC7A20写一个字节
bool SC7A20_IIC_WriteReg(uint8_t dev_id, uint8_t address, uint8_t *pDataBuff, uint8_t length)
{
	ret_code_t err_code;
	uint8_t write_data[2];
	write_data[0] = address;
	memcpy(&write_data[1], pDataBuff, length);
	//TWI传输完成标志设置为false
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
*函数        	:	bool SC7A20_Init(SC7A20_HANDLE *pHandle, uint8_t SlaveAddr, 
						bool (*IIC_ReadReg)(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *pDataBuff, uint16_t ByteNum), 
						bool (*IIC_WriteReg)(uint16_t SlaveAddr, uint8_t RegAddr, uint8_t *pDataBuff, uint16_t ByteNum))
*功能        	:	SC7A20初始化
*参数        	:	pHandle:句柄；SlaveAddr：芯片IIC写地址；IIC_ReadReg：IIC读取接口；IIC_WriteReg:IIC写入接口；
*返回        	:	1:初始化成功；0:初始化失败
*依赖			: 	底层宏定义
*作者       		:	cp1300@139.com
*时间     		:	2022-07-01
*最后修改时间		:	2022-07-01
*说明        	:	
*************************************************************************************************************************/
bool SC7A20_Init(void)
{
	uint8_t tempreg = 0;

  nrf_gpio_pin_write(SC7A20_CS, 1);
	nrf_gpio_pin_write(SC7A20_SDA, 1);
	nrf_gpio_pin_write(SC7A20_SCL, 1);

	//nrf的twi初始化
	ret_code_t err_code;
	err_code = nrf_drv_twi_init(&m_twi, &twi_sc7a20_config, twi_handler, NULL);
	APP_ERROR_CHECK(err_code);
	nrf_drv_twi_enable(&m_twi);
 
	nrf_delay_ms(100);
	
	//读WHO_AM_I
	read_buffer[0] = 0;
	init_state = SC7A20_IIC_ReadReg(SC7A20_Address, SC7A20_REG_WHO_AM_I, &read_buffer[0], 1);
	if(read_buffer[0] == 0x11)
		init_state = 1;
	else
		init_state = 0;
	nrf_delay_ms(100);


	tempreg = 0x47;
	SC7A20_IIC_WriteReg(SC7A20_Address, SC7A20_REG_CTRL_1, &tempreg, 1);//50Hz+正常模式xyz使能
	tempreg = 0x00;
	SC7A20_IIC_WriteReg(SC7A20_Address, SC7A20_REG_CTRL_2, &tempreg, 1);//关闭滤波器，手册上面没有滤波器截止频率设置说明，开启后无法测量静止状态下的重力加速度
	tempreg = 0x00;
	SC7A20_IIC_WriteReg(SC7A20_Address, SC7A20_REG_CTRL_3, &tempreg, 1); //关闭中断
	tempreg = 0x88;
	SC7A20_IIC_WriteReg(SC7A20_Address, SC7A20_REG_CTRL_4, &tempreg, 1); //读取完成再更新，小端模式，、2g+正常模式，高精度模式

	return 0;
}
 
short SC7A20_12bitComplement(uint8_t msb, uint8_t lsb)
{
	short temp;
 
	temp = msb << 8 | lsb;
	temp = temp >> 4;   //只有高12位有效
	temp = temp & 0x0fff;
	if (temp & 0x0800) //负数 补码==>原码
	{
		temp = temp & 0x07ff; //屏弊最高位      
		temp = ~temp;
		temp = temp + 1;
		temp = temp & 0x07ff;
		temp = -temp;       //还原最高位
	}
	return temp;
}
 
 
/*************************************************************************************************************************
*函数        	:	bool SC7A20_ReadAcceleration(SC7A20_HANDLE* pHandle, short* pXa, short* pYa, short* pZa)
*功能        	:	SC7A20读取加速度值
*参数        	:	pHandle:句柄；pXa，pYa,pZa 三轴加速度值
*返回        	:	1:成功；0:失败
*依赖			: 	底层宏定义
*作者       		:	cp1300@139.com
*时间     		:	2022-07-01
*最后修改时间		:	2022-07-01
*说明        	:
*************************************************************************************************************************/
bool SC7A20_ReadAcceleration(short* pXa, short* pYa, short* pZa)
{
	uint8_t buff[6] = {0,0,0,0,0,0};
//	uint8_t i;
 
	memset(buff, 0, 6);

	SC7A20_IIC_ReadReg(SC7A20_Address, SC7A20_REG_X_L|0x80, buff, 6);

	//X轴
	*pXa = buff[1];
	*pXa <<= 8;
	*pXa |= buff[0];
	*pXa >>= 4;	//取12bit精度

	//Y轴
	*pYa = buff[3];
	*pYa <<= 8;
	*pYa |= buff[2];
	*pYa >>= 4;	//取12bit精度
	
	//Z轴
	*pZa = buff[5];
	*pZa <<= 8;
	*pZa |= buff[4];
	*pZa >>= 4;	//取12bit精度

	return 1;
 
}
 
#define PI 3.1415926535898
/*************************************************************************************************************************
*函数        	:	bool SC7A20_GetZAxisAngle(SC7A20_HANDLE* pHandle, short AcceBuff[3], float* pAngleZ)
*功能        	:	SC7A20 获取Z轴倾角
*参数        	:	pHandle:句柄；AcceBuff:3个轴的加速度；pAngleZ：Z方向倾角
*返回        	:	1:成功；0:失败
*依赖			: 	底层宏定义
*作者       		:	cp1300@139.com
*时间     		:	2022-07-02
*最后修改时间		:	2022-07-02
*说明        	:
*************************************************************************************************************************/
bool SC7A20_GetZAxisAngle(short AcceBuff[3], float* pAngleZ)
{
	double fx, fy, fz;
	double A;
	short Xa, Ya, Za;
  SC7A20_ReadAcceleration(&Xa, &Ya, &Za);
	//uart_printf("Xa:%d \tYa:%d \tZa:%d \r\n",Xa,Ya,Za);
	AcceBuff[0] = Xa;	//x轴加速度
	AcceBuff[1] = Ya;	//y轴加速度
	AcceBuff[2] = Za;	//z轴加速度
 
	fx = Xa;
	fx *= 2.0 / 4096;
	fy = Ya;
	fy *= 2.0 / 4096;
	fz = Za;
	fz *= 2.0 / 4096;
 
	//uart_printf("fx：%.04f\tfy：%.04f\tfz：%.04f\t\r\n",fx,fy,fz);
 
	//Z方向
	A = fx * fx + fy * fy;
	A = sqrt(A);
	A = (double)A / fz;
	A = atan(A);
	A = A * 180 / PI;
 
	*pAngleZ = A;
	//uart_printf("=======角度：%.04f\r\n",*pAngleZ);
 
 
	return 1;
}

