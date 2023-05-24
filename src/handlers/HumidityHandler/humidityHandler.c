#include <stdint.h>
#include "humidityHandler.h"
#include <ATMEGA_FreeRTOS.h>
#include "task.h"
#include "../src/implementation/humidityImpl/humidityImpl.h"
#include "sensorData.h"
#include <queue.h>

extern QueueHandle_t xQueue;
TaskHandle_t humidityHandlerTaskHandle = NULL;

void humidity_Init(){
	create_humidityhandler_task();
}

void humidity_handler_task(void *pvParameters)
{
    SensorData data;
    data.sensorName = "Humidity";
    
    for(;;)
    {
        humimpl_measure();
        uint16_t humValue = humimpl_getMeasurement();

        data.status = (humValue != 0) ? SENSOR_STATUS_OK : SENSOR_STATUS_ERROR; 
        data.data = humValue;

        xQueueSend(xQueue, &data, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(1000));  
    }
}

void create_humidityhandler_task()
{
    BaseType_t taskCreated;
    taskCreated = xTaskCreate(
        humidity_handler_task,
        "humidity_handler_task",
        configMINIMAL_STACK_SIZE+100,
        NULL,
        0,
        &humidityHandlerTaskHandle
    );


}


TaskHandle_t humidityHandlerTaskHandle = NULL;

void humidity_handler_task(void *pvParameters)
{
    SensorData data;
    data.sensorName = "Humidity";
    
    for(;;)
    {
        humimpl_measure();
        uint16_t humValue = humimpl_getMeasurement();

        data.status = (humValue != 0) ? SENSOR_STATUS_OK : SENSOR_STATUS_ERROR; 
        data.data = humValue;

        xQueueSend(sensorQueue, &data, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(1000));  
    }
}

void create_humidityhandler_task()
{
    BaseType_t taskCreated;
    taskCreated = xTaskCreate(
        humidity_handler_task,
        "humidity_handler_task",
        1000,
        NULL,
        1,
        &humidityHandlerTaskHandle
    );


}

uint16_t hum_getMeasurement(){
	humimpl_measure();
    
    return humimpl_getMeasurement();
}
