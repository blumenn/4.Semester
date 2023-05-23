#include <stdint.h>
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
    
    
    for(;;)
    {
        humimpl_measure();
        uint16_t humValue = humimpl_getMeasurement();
		data.sensorName = hum;
        data.status = (humValue != 0) ? SENSOR_STATUS_OK : SENSOR_STATUS_ERROR; 
        data.data = humValue;

        xQueueSend(xQueue, &data, 10);

        vTaskDelay(pdMS_TO_TICKS(5000));  
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


uint16_t hum_getMeasurement(){
	humimpl_measure();
    
    return humimpl_getMeasurement();
}
