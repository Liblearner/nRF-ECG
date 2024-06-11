/**
 * Copyright (c) 2015 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "main.h"

#if LEDS_NUMBER <= 2
#error "Board is not equipped with enough amount of LEDs"
#endif

//adc读取buffer
int16_t adc_value;
float adc_MVA;
float adc_LPF;

//移动平均结构体
t_MAF MVA_data;


//acc读取buffer
short acc[3] = {0,0,0};
float zAngle = 0;

//SC7A20初始化读取状态yu buffer
uint8_t init_state = 0;
uint8_t read_buffer[16] = 
{0,0,0,0
,0,0,0,0
,0,0,0,0
,0,0,0,0};

static void hardware_init(void);

int main(void)
{
		/*nRF与外设初始化*/
		hardware_init();
	


	
	
	  /*PT结构体初始化*/
		PT_init();
		moving_average_filter_init(&MVA_data);

    while (true)
    {
     //SC7A20_GetZAxisAngle(acc,&zAngle);
			//nrf_delay_ms(20);
//			for(int8_t ii=0; ii<0x02;ii=ii+1)
//			{
//				for (int8_t jj=0;jj<0x02;jj=jj+1)
//				{
//					for (int8_t kk=0;kk<0x02;kk=kk+1)
//					{
//						nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(0,29),ii);
//						nrf_gpio_pin_write(AD8233_FR, jj);
//						nrf_gpio_pin_write(AD8233_SW_EN, kk);
//						nrf_delay_ms(5000);
//					}}}
			
			
    }
}

	
static void hardware_init(void)
{
    ret_code_t err_code;
	
    /* Initialize clock driver for better time accuracy in FREERTOS */
//    err_code = nrf_drv_clock_init();
//    APP_ERROR_CHECK(err_code);

    /* Configure LED-pins as outputs */
//    bsp_board_init(BSP_INIT_LEDS);

    /*sc7a20初始化*/
    //SC7A20_Init();
	
		/*ad8233初始化*/
		ad8233_init();
	
}
