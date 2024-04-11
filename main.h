#ifndef MAIN_H
#define MAIN_H

//standard lib
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
#include "sdk_errors.h"
#include "app_error.h"
#include "nrf_delay.h"

//hardware
#include "ad8233.h"

//algorithm
#include "PanTompkins.h"

//task
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "Task_PanTompkins.h"
#include "Task_led.h"
#include "start_tasks.h"

#endif
