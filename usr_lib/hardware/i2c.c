/*********************************************************************
 * INCLUDES
 */


#include "i2c.h"

static void twi_handleEvent(nrf_drv_twi_evt_t const *pEvent, void *pContext);
static void mergeRegisterAndData(uint8_t *pTxBuf, uint8_t regAddr, uint8_t *pData, uint16_t dataLen);

/*********************************************************************
 * LOCAL VARIABLES
 */
static const nrf_drv_twi_t s_twiHandle = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

volatile static bool s_twiTxDone = false;                   // ������ɱ�־
volatile static bool s_twiRxDone = false;                   // ������ɱ�־   

static uint8_t s_twiWriteDataBuffer[TWI_MAX_NUM_TX_BYTES];

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/**
 @brief TWI(I2C)������ʼ��
 @param ��
 @return ��
*/
void I2C_Init(void)
{
    uint32_t errCode;
    
    // ��ʼ��TWI���ýṹ��
    const nrf_drv_twi_config_t twiConfig =
    {
        .scl                = BOARD_TWI_SCL_IO,             // ����TWI SCL����
        .sda                = BOARD_TWI_SDA_IO,             // ����TWI SDA����
        .frequency          = NRF_TWI_FREQ_400K,            // ����TWIʱ��Ƶ��
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH         // TWI�ж����ȼ�����
    };
    
    // ��ʼ��TWI
    errCode = nrf_drv_twi_init(&s_twiHandle, &twiConfig, twi_handleEvent, NULL);
    APP_ERROR_CHECK(errCode);
    
    // ʹ��TWI
    nrf_drv_twi_enable(&s_twiHandle);
}

/**
 @brief TWI(I2C)д���ݺ���
 @param slaveAddr -[in] ���豸��ַ
 @param regAddr -[in] �Ĵ�����ַ
 @param pData -[in] д������
 @param dataLen -[in] д�����ݳ���
 @return ������
*/
uint16_t I2C_WriteData(uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint16_t dataLen)
{
    // This burst write function is not optimal and needs improvement.
    // The new SDK 11 TWI driver is not able to do two transmits without repeating the ADDRESS + Write bit byte
    uint32_t errCode;
    uint32_t timeout = TWI_TIMEOUT;
    // Merging MPU register address and p_data into one buffer.
    mergeRegisterAndData(s_twiWriteDataBuffer, regAddr, pData, dataLen);
    // Setting up transfer
    nrf_drv_twi_xfer_desc_t xferDesc;
    xferDesc.address = slaveAddr;
    xferDesc.type = NRF_DRV_TWI_XFER_TX;
    xferDesc.primary_length = dataLen + 1;
    xferDesc.p_primary_buf = s_twiWriteDataBuffer;

    // Transferring
    errCode = nrf_drv_twi_xfer(&s_twiHandle, &xferDesc, 0);

    while((!s_twiTxDone) && --timeout);
    if(!timeout)
    {
        return NRF_ERROR_TIMEOUT;
    }
    s_twiTxDone = false;

    return errCode; 
}

/**
 @brief TWI(I2C)�����ݺ���
 @param slaveAddr -[in] ���豸��ַ
 @param regAddr -[in] �Ĵ�����ַ
 @param pData -[in] ��������
 @param dataLen -[in] �������ݳ���
 @return ������
*/
uint16_t I2C_ReadData(uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint16_t dataLen)
{
    uint32_t errCode;
    uint32_t timeout = TWI_TIMEOUT;

    errCode = nrf_drv_twi_tx(&s_twiHandle, slaveAddr, &regAddr, 1, false);
    if(errCode != NRF_SUCCESS)
    {
        return errCode;
    }

    while((!s_twiTxDone) && --timeout); 
    if(!timeout)
    {
        return NRF_ERROR_TIMEOUT;
    }
    s_twiTxDone = false;

    errCode = nrf_drv_twi_rx(&s_twiHandle, slaveAddr, pData, dataLen);
    if(errCode != NRF_SUCCESS)
    {
        return errCode;
    }

    timeout = TWI_TIMEOUT;
    while((!s_twiRxDone) && --timeout);
    if(!timeout)
    {
        return NRF_ERROR_TIMEOUT;
    }
    s_twiRxDone = false;
    
    return errCode;
}

/**
 @brief ����TWI(I2C)
 @param ��
 @return ��
*/
void I2C_Enable(void)
{
    nrf_drv_twi_enable(&s_twiHandle);
}

/**
 @brief ����TWI(I2C)
 @param ��
 @return ��
*/
void I2C_Disable(void)
{
    nrf_drv_twi_disable(&s_twiHandle);
}


/*********************************************************************
 * LOCAL FUNCTIONS
 */
/**
 @brief TWI�¼�������
 @param pEvent -[in] TWI�¼�
 @return ��
*/
static void twi_handleEvent(nrf_drv_twi_evt_t const *pEvent, void *pContext)
{
    // �ж�TWI�¼�����
    switch(pEvent->type)
    {
        // ��������¼�
        case NRF_DRV_TWI_EVT_DONE:
            switch(pEvent->xfer_desc.type)
            {
                case NRF_DRV_TWI_XFER_TX:
                    s_twiTxDone = true;
                    break;
                case NRF_DRV_TWI_XFER_TXTX:
                    s_twiTxDone = true;
                    break;
                case NRF_DRV_TWI_XFER_RX:
                    s_twiRxDone = true;
                    break;
                case NRF_DRV_TWI_XFER_TXRX:
                    s_twiRxDone = true;
                    break;
                default:
                    break;
            }
            break;
        
        case NRF_DRV_TWI_EVT_ADDRESS_NACK:
            break;
        
        case NRF_DRV_TWI_EVT_DATA_NACK:
            break;
        
        default:
            break;
    }   
}

/**
 @brief �ϲ��Ĵ�����ַ�ʹ�д������
 @param pTxBuf -[in] д�뻺����
 @param regAddr -[in] �Ĵ�����ַ
 @param pData -[in] д������
 @param dataLen -[in] д�����ݳ���
 @return ��
*/
static void mergeRegisterAndData(uint8_t *pTxBuf, uint8_t regAddr, uint8_t *pData, uint16_t dataLen)
{
    pTxBuf[0] = regAddr;
    memcpy((pTxBuf + 1), pData, dataLen);
}

/****************************************************END OF FILE****************************************************/




