#include "main.h"
#include "Task_led.h"

/**@brief LED0 task entry function.
 *
 * @param[in] pvParameter   Pointer that will be used as the parameter for the task.
 */
void led_toggle_task_function (void)
{
    while (true)
    {
        //bsp_board_led_invert(BSP_BOARD_LED_0);
	  adc_value = m_buffer_pool[0][0];
		PT_StateMachine(adc_value);
        /* Delay a task for a given number of ticks */
        vTaskDelay(5);

        /* Tasks must be implemented to never return... */
    }
}

/**@brief The function to call when the LED1 FreeRTOS timer expires.
 *
 * @param[in] pvParameter   Pointer that will be used as the parameter for the timer.
 */
void led_toggle_timer_callback (void)
{
    //bsp_board_led_invert(BSP_BOARD_LED_1);

}

