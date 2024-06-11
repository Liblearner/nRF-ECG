#ifndef MAIN_H
#define MAIN_H

//standard lib
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//board lib
#include "nrf.h"
#include "bsp.h"
#include "boards.h"
#include "nordic_common.h"

//drive lib
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_twi.h"
#include "sdk_errors.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf_mpu_lib.h"

//algorithm
#include "PanTompkins.h"
#include "filter.h"
#include "wavelib.h"

//hardware
#include "ad8233.h"
#include "sc7a20.h"

//task
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "Task_PanTompkins.h"
#include "Task_led.h"
#include "start_tasks.h"

#endif

extern int16_t adc_value;
extern uint8_t init_state;
extern uint8_t read_buffer[16];
extern t_MAF MVA_data;
extern float adc_MVA;
extern float adc_LPF;


