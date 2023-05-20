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
uint16_t co2_ppm = co2_getMeasurement();
uint16_t temp = temp_getMeasurement();
uint16_t hum = hum_getMeasurement();

	_uplink_payload.bytes[0] = hum >> 8;
	_uplink_payload.bytes[1] = hum & 0xFF;
	_uplink_payload.bytes[2] = temp >> 8;
	_uplink_payload.bytes[3] = temp & 0xFF;
	_uplink_payload.bytes[4] = co2_ppm >> 8;
	_uplink_payload.bytes[5] = co2_ppm & 0xFF;
return _uplink_payload;
	
}



