#include "main.h"
#include "Task_PanTompkins.h"

/**@brief PT task entry function.
 *
 * @param[in] pvParameter   Pointer that will be used as the parameter for the task.
 */
 

void PanTompkins_task (void)
{
    while (true)
    {
        

        /* Delay a task for a given number of ticks */
        PT_StateMachine(m_buffer_pool[0][0]);
			  vTaskDelay(5);
        /* Tasks must be implemented to never return... */
    }
}

