#include "ad8233.h"

static const nrf_drv_timer_t m_timer = NRF_DRV_TIMER_INSTANCE(0);
nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];//��static֮����Ҳ���
static nrf_ppi_channel_t     m_ppi_channel;
static uint32_t              m_adc_evt_counter;

/*����Ҫ����������֮��ϵͳ�Զ����ߵĹ���*/

void timer_handler(nrf_timer_event_t event_type, void * p_context)
{

}

void saadc_sampling_event_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&m_timer, &timer_cfg, timer_handler);
    APP_ERROR_CHECK(err_code);

	/*��������޸Ķ�ʱ���ļ���ʱ�䣬��λms*/
	
    /* setup m_timer for compare event every x ms */
    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer, 5);
    nrf_drv_timer_extended_compare(&m_timer,
                                   NRF_TIMER_CC_CHANNEL0,
                                   ticks,
                                   NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                   false);
    nrf_drv_timer_enable(&m_timer);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer,
                                                                                NRF_TIMER_CC_CHANNEL0);
    uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel,
                                          timer_compare_event_addr,
                                          saadc_sample_task_addr);
    APP_ERROR_CHECK(err_code);
}


void saadc_sampling_event_enable(void)
{
    ret_code_t err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);
}


void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        ret_code_t err_code;

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);

//        int i;
        //NRF_LOG_INFO("ADC event number: %d", (int)m_adc_evt_counter);

//        for (i = 0; i < SAMPLES_IN_BUFFER; i++)
//        {
//            NRF_LOG_INFO("%d", p_event->data.done.p_buffer[i]);
//        }
        m_adc_evt_counter++;
    }
}


void saadc_init(void)
{
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

}


void ad8233_init(void)
{
	  nrf_gpio_cfg_output(AD8233_FR);
	  nrf_gpio_cfg_output(AD8233_SW_EN);
	
		nrf_gpio_pin_write(AD8233_FR, 1);
		nrf_gpio_pin_write(AD8233_SW_EN, 1);
		
	  /*�缫���ź�ʱLOD���Ϊ0����������Ϊ1�����LOD����Ϊ����*/
	  nrf_gpio_cfg_input(AD8233_LOD,NRF_GPIO_PIN_PULLDOWN);
	
		//ADC��ADC��ȡ�¼���ʼ��
		saadc_init();
    saadc_sampling_event_init();
    saadc_sampling_event_enable();
}

