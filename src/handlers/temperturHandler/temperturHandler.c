
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include <queue.h>
#include "../../src/implementation/tempImpl/temperaturImpl.h"
#include "sensorData.h"
#include "temperturHandler.h"

extern QueueHandle_t xQueue;

void temperature_handler_task(void *pvParameters)
{
    SensorData data;
    data.sensorName =temp;
    for(;;)
    {
        tempimpl_measure();
        uint16_t tempValue = tempimpl_getMeasurement();
        data.status = (tempValue != 0) ? SENSOR_STATUS_OK : SENSOR_STATUS_ERROR;
        data.data = tempValue;

        xQueueSend(xQueue, &data, 100);

        vTaskDelay(pdMS_TO_TICKS(5000));  
        }
}

void create_temperaturehandler_task()
{
    BaseType_t taskCreated;
    taskCreated = xTaskCreate(
        temperature_handler_task,
        "temperature_handler_task",
        configMINIMAL_STACK_SIZE+100,
        NULL,
        2,
        NULL
    );
}

void temp_init()
{
    tempimpl_init();
    create_temperaturehandler_task();
}

uint16_t temp_getMeasurement()
{
    tempimpl_measure();
    return tempimpl_getMeasurement();
}
