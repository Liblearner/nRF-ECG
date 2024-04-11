#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "start_tasks.h"

//prio��stack��handle��dealys of tasks and timers
#define START_TASK_PRIO 1
#define START_STK_SIZE configMINIMAL_STACK_SIZE
TaskHandle_t StartTask_Handler = NULL;

#define task_led_toogle_PRIO 30
#define task_led_toogle_SIZE configMINIMAL_STACK_SIZE + 200
TaskHandle_t led_toggle_task_handle;

#define task_PanTompkins_SIZE 30
#define task_PanTompkins_PRIO 512
TaskHandle_t task_PanTompkins_handle;

TimerHandle_t led_toggle_timer_handle;  /**< Reference to LED1 toggling FreeRTOS timer. */

#define TIMER_PERIOD      1000          /**< Timer period. LED1 timer will expire after 1000 ms */


//��������
static void start_task(void *pvParameters)
{
    //BaseType_t xReturn = pdPASS;
    
    taskENTER_CRITICAL();//�ٽ�α���

	    /* Create task for LED0 blinking with priority set to 2 */
    UNUSED_VARIABLE(xTaskCreate(
										(TaskFunction_t)led_toggle_task_function, 
										"LED0", 
										task_led_toogle_SIZE, 
										NULL, 
										task_led_toogle_PRIO, 
										&led_toggle_task_handle));

    /* Start timer for LED1 blinking */
    led_toggle_timer_handle = xTimerCreate( "LED1", 
										TIMER_PERIOD, 
										pdTRUE, 
										NULL, 
										(TimerCallbackFunction_t)led_toggle_timer_callback);
    
	  UNUSED_VARIABLE(xTimerStart(led_toggle_timer_handle, 0));
	
		UNUSED_VARIABLE(xTaskCreate(
										(TaskFunction_t)PanTompkins_task,
										"PanTompkins",
										task_PanTompkins_SIZE,
										NULL,
										task_PanTompkins_PRIO,
										&task_PanTompkins_handle));
	                
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ��
}

void start_the_very_first_task(void)
{
    //������������
    xTaskCreate((TaskFunction_t)start_task,          //������
                (const char *)"start_task",          //��������
                (uint16_t)START_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler); //������
}
