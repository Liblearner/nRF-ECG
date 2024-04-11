#ifndef AD8233_H
#define AD8233_H

#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "main.h"
/*
AD8233：
ECG_ADC 			P0.02：ECG模拟信号输入
8233_FR 			P0.03：内部带通配置
8233_ACDC 				 ：耦合方式配置
8233_SW_EN 		P1.15：开关使能？
8233_LOD 			P1.14：陀螺检测
*/

//IO定义
#define AD8233_FR          	NRF_GPIO_PIN_MAP(0,3)
#define AD8233_SW_EN        NRF_GPIO_PIN_MAP(1,15)
#define AD8233_LOD          NRF_GPIO_PIN_MAP(1,14)

#define SAMPLES_IN_BUFFER 5

////采样buffer
extern nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];


void ad8233_init(void);

#endif


