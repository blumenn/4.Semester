#include <stdint.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "co2Handler.h"
#include "SensorData.h"
#include "../../implementation/co2Impl/co2.h"
#include "mh_z19.h"
#include "task.h"


void co2_handler_task(void *pvParameters);

TaskHandle_t co2HandlerTaskHandle = NULL;

void create_co2handler_task()
{
	BaseType_t taskCreated;
	taskCreated = xTaskCreate(
	co2_handler_task,
	"co2_handler_task",
	1000,
	NULL,
	1,
	&co2HandlerTaskHandle
	);
}

void run_co2handler_task()
{
	SensorData data;
	data.sensorName = "Co2Sensor";
	mh_z19_returnCode_t returnCode = co2impl_measure();
	data.status = (returnCode == MHZ19_OK) ? SENSOR_STATUS_OK : SENSOR_STATUS_ERROR;
	data.data = co2impl_getMeasurement();
	xQueueSend(sensorQueue, &data, portMAX_DELAY);
}

void co2_handler_task(void *pvParameters)
{
	for(;;)
	{
		run_co2handler_task();
		vTaskDelay(pdMS_TO_TICKS(10000));
	}
}

void co2_init()
{
	co2impl_init();
	create_co2handler_task();
}

uint16_t co2_getMeasurement()
{
	return co2impl_getMeasurement();
}
