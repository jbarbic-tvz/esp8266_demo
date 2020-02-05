/*
 * Some ESP 8266 demo
 *  - RTOS implementation of queues, output
 * 
 * WARNING: Code is not tested on device
 */

#include "esp_common.h"
#include "gpio.h"

/* Global handle */
QueueHandle_t qhand1 = 0;

/* Enviroment */
#define GPIO_PORT 3
#define FLASH_DELAY 100
#define CYCLE_DELAY 10000

void task_call(void* p)
{
    int counter_sub = 0;
    while(1)
    {
        counter_sub++;
        if(!xQueueSend(qhand1, &counter_sub, CYCLE_DELAY)) {
            /* Do something on timeout ... failed */
            printf("Failed\n");
        }
        vTaskDelay(CYCLE_DELAY);
    }
}

void task_exe(void* p)
{
    int counter_sub = 0;
    while(1)
    {
        if(!xQueueReceive(qhand1, &counter_sub, CYCLE_DELAY)) {
            /* Do something on timeout ... failed */
            printf("Failed\n");
        }
        else {
            OUTPUT_GPIO (GPIO_PORT, 0);
		    vTaskDelay(FLASH_DELAY);
		    OUTPUT_GPIO (GPIO_PORT, 1);
        }
    }
}

int main()
{
    qhand1 = xQueueCreate(1, sizeof(int));
    /* First task */
    xTaskCreate(task_call, (signed char*)"t1", STACK_BYTES(2048), 0, 2, 0);
    /* Called task */
    xTaskCreate(task_exe, (signed char*)"t2", STACK_BYTES(2048), 0, 1, 0);

    vTaskStartScheduler();

    return -1;
}
