#include <stdint.h>
#include "./src/handlers/co2Handler/interface/co2Handler.h"
#include "./src/handlers/HumidityHandler/humidityHandler.h"
#include "./src/handlers/temperturHandler/temperturHandler.h"
#include "./InterfaceWrapper/Wrapper.h"
#include <lora_driver.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <status_leds.h>


static lora_driver_payload_t _uplink_payload;
QueueHandle_t xQueue;
static latestData latestData; 
static measuringSum tempSum;
static measuringSum humSum;
static measuringSum co2Sum;
void wrapper_init(){
	_uplink_payload.len = 6;
	_uplink_payload.portNo = 2;
	xQueue = xQueueCreate(15,sizeof(SensorData *));
	if (xQueue == NULL)
	{
		// return fejl
		return
	}

	
	
}

 lora_driver_payload_t wrapperhandler()
{
uint16_t co2_ppm = avg_x10(co2Sum);
uint16_t temp = avg_x10(tempSum);
uint16_t hum = avg_x10(humSum);

	_uplink_payload.bytes[0] = hum >> 8;
	_uplink_payload.bytes[1] = hum & 0xFF;
	_uplink_payload.bytes[2] = temp >> 8;
	_uplink_payload.bytes[3] = temp & 0xFF;
	_uplink_payload.bytes[4] = co2_ppm >> 8;
	_uplink_payload.bytes[5] = co2_ppm & 0xFF;
return _uplink_payload;	
}
wrapper_task(){
	SensorData data;
	if( xQueue != NULL ) //checks if queue is made
   {
     while (xQueueIsQueueEmptyFromISR(xQueue)== pdFAIL)
	 {
		if( xQueueReceive( xQueue,
                         &( data ),
                         ( TickType_t ) 100 ) == pdPASS )
      {

         saveData(data);
		 
      }
	 }
	 
      
   }
}


void saveData(SensorData data){
	if(data.status==SENSOR_STATUS_OK){
	if (data.sensorName=="Co2Sensor")
	{
	latestData.co2 = data;
	co2Sum.antal +=1;
	co2Sum.sum += data.data;
	return;
	}
	if (data.sensorName == "Humidity")
	{
		latestData.hum = data;
		humSum.antal +=1;
		humSum.sum += data.data;
		return;
	}
	if (data.sensorName == "Temperature")
	{
		latestData.temp = data;
		tempSum.antal +=1;
		tempSum.sum += data.data;
		return;
	}
	return;

}

}
int16_t avg_x10(measuringSum data) {
	int16_t result= data.sum*10/data.antal;
	data.antal=0;
	data.sum = 0;
	return result;
}

latestData get_latestData(){
	return latestData;
}
