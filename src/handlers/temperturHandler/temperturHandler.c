#include <stdint.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "../../src/implementation/tempImpl/temperaturImpl.h"
#include "sensorData.h"
#include "temperturHandler.h"
#include <queue.h>

extern QueueHandle_t xQueue;

TaskHandle_t temperatureHandlerTaskHandle = NULL;

void temperature_handler_task(void *pvParameters)
{
    SensorData data;
    data.sensorName = "Temperature";
    
    for(;;)
    {
        tempimpl_measure();
        uint16_t tempValue = tempimpl_getMeasurement();

        data.status = (tempValue != 0) ? SENSOR_STATUS_OK : SENSOR_STATUS_ERROR;
        data.data = tempValue;

       
        xQueueSend(sensorQueue, &data, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(1000));  
        }
}

void create_temperaturehandler_task()
{
    BaseType_t taskCreated;
    taskCreated = xTaskCreate(
        temperature_handler_task,
        "temperature_handler_task",
        1000,
        NULL,
        1,
        &temperatureHandlerTaskHandle
    );


}

void temp_init()
{
    tempimpl_init();
    create_temperaturehandler_task();
}

uint16_t temp_getMeasurement(){
    
tempimpl_measure();

return tempimpl_getMeasurement();
}
