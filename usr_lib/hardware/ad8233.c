#include "ad8233.h"

static const nrf_drv_timer_t m_timer = NRF_DRV_TIMER_INSTANCE(0);
int16_t     m_buffer_pool[1][SAMPLES_IN_BUFFER];//��static֮����Ҳ���
static nrf_ppi_channel_t     m_ppi_channel;
static int8_t ii = 0;

//С���任���
wave_object obj;  //С����������ʼ��
wt_object wt;  //�任���ʼ��
double *inp,*out,*diff;  //diffΪԭ�ź���任���ź�������ֵ�������ڼ���
int N, i,J;  
double temp[1200];  
char *name = "db4";  


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
    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer, 1);
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
				adc_value = m_buffer_pool[0][0];
				adc_MVA = moving_average_filter(&MVA_data,adc_value);
				adc_LPF = low_pass_filter(adc_value);
			
				/*С���任���*/
				obj = wave_init(name);// ͨ�����ֳ�ʼ��С������
				//һ�ν���С���任���źų��ȣ�����Ϊ256ʱ��1kHz���ܲ���
				N= 64;
				inp = (double*)malloc(sizeof(double)* N);  
				out = (double*)malloc(sizeof(double)* N);  
				diff = (double*)malloc(sizeof(double)* N);
				J = 3;  
				wt = wt_init(obj, "dwt", N, J);
				setDWTExtension(wt, "sym");  
				setWTConv(wt, "direct");
//				dwt(wt, inp);//DWT�任  
//				/*�����˲�*/
//				//DWT output can be accessed using wt->output vector. Use wt_summary to  find out how to extract appx and detail coefficients 	
//				//outû�и�ֵ��ֱ��������任��
//				out = wt->output;
//				
//				
//				idwt(wt, out); 
				//��̬�ڴ��ͷ�
				wave_free(obj);  
				wt_free(wt);  
				free(inp);  
				free(out);  
				free(diff); 
			
			
			  if(ii%1 == 0)
					PT_StateMachine(adc_value);
				ii++;

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
//    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLES_IN_BUFFER);
//    APP_ERROR_CHECK(err_code);

}


void ad8233_init(void)
{
	  nrf_gpio_cfg_output(AD8233_FR);
	  nrf_gpio_cfg_output(AD8233_SW_EN);
	  nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0,29));
	 	nrf_delay_ms(100);		
	  nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(0,29),1);
		nrf_gpio_pin_write(AD8233_FR, 1);
		nrf_gpio_pin_write(AD8233_SW_EN, 1);

		low_pass_filter_init();
	
		//ADC��ADC��ȡ�¼���ʼ��
		saadc_init();
    saadc_sampling_event_init();
    saadc_sampling_event_enable();

	
	
}

