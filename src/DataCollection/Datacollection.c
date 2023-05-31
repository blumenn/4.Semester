#include "../src/handlers/co2Handler/co2Handler.h"
#include "../src/handlers/HumidityHandler/humidityHandler.h"
#include "../src/handlers/temperturHandler/temperturHandler.h"
#include "datacollection.h"
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <status_leds.h>

static lora_driver_payload_t _uplink_payload;
QueueHandle_t xQueue;
static latestData lastData; 
static measuringSum tempSum;
static measuringSum humSum;
static measuringSum co2Sum;
static SemaphoreHandle_t servoWrapperSemaphore=NULL;

void dataCollection_init(){
	_uplink_payload.len = 6;
	_uplink_payload.portNo = 2;
	xQueue = xQueueCreate(15,sizeof(SensorData));
	create_dataCollection_task();
	if ( servoWrapperSemaphore == NULL )  
	{
		servoWrapperSemaphore = xSemaphoreCreateMutex();  
		if ( ( servoWrapperSemaphore ) != NULL )
		{
			xSemaphoreGive( ( servoWrapperSemaphore ) );  
		}
	}
	if (xQueue == NULL)
	{
		return;
	}
}

 lora_driver_payload_t wrapperhandler()
{
uint16_t co2_ppm = avg(&co2Sum);
uint16_t temp = avg(&tempSum);
uint16_t hum = avg(&humSum);

	_uplink_payload.bytes[0] = hum >> 8;
	_uplink_payload.bytes[1] = hum & 0xFF;
	_uplink_payload.bytes[2] = temp >> 8;
	_uplink_payload.bytes[3] = temp & 0xFF;
	_uplink_payload.bytes[4] = co2_ppm >> 8;
	_uplink_payload.bytes[5] = co2_ppm & 0xFF;
return _uplink_payload;	
}

dataCollection_task( ){
	SensorData data;
	if( xQueue != NULL ) 
   {
     while (xQueueReceive( xQueue,&( data ),100 ) == pdPASS)
	 {
         saveData(data);
	 }
      return;
   }
   }

run_dataCollection_task(void *pvParameters){
	for(;;){
		dataCollection_task();
		vTaskDelay(pdMS_TO_TICKS(5000));
	}
}

void create_dataCollection_task()
{
	BaseType_t taskCreated;
	taskCreated = xTaskCreate(
	run_dataCollection_task,
	"dataCollection_task",
	configMINIMAL_STACK_SIZE+100,
	NULL,
	4,
	NULL
	);
}

void saveData(SensorData data){
	if (xSemaphoreTake(servoWrapperSemaphore,pdMS_TO_TICKS(2000))==pdTRUE)
	{
		if(data.status==SENSOR_STATUS_OK){
			if (data.sensorName==Co2Sensor)
			{
			lastData.co2 = data;
			co2Sum.antal +=1;
			co2Sum.sum += data.data;
			xSemaphoreGive(servoWrapperSemaphore);
			return;
			}
			if (data.sensorName==hum)
			{
				lastData.hum = data;
				humSum.antal +=1;
				humSum.sum += data.data;
				xSemaphoreGive(servoWrapperSemaphore);
				return;
			}
			if (data.sensorName==temp)
			{
				lastData.temp = data;
				tempSum.antal +=1;
				tempSum.sum += data.data;
				xSemaphoreGive(servoWrapperSemaphore);
				return;
			}
			xSemaphoreGive(servoWrapperSemaphore);
		return;
		}
	}
}

int16_t avg(measuringSum *data) {
	if (xSemaphoreTake(servoWrapperSemaphore,pdMS_TO_TICKS(2000))==pdTRUE)
		{
		int16_t result= data->sum/data->antal;
		data->antal=0;
		data->sum = 0;
		xSemaphoreGive(servoWrapperSemaphore);
		return result;
		}
	return 0;
}

latestData get_latestData(){
	return lastData;
}



