#include <stdint.h>
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include "humidityImpl.h"
#include "delay_hal.h"

static uint16_t hum;
extern SemaphoreHandle_t xTestSemaphore;




void humimpl_measure(){
    if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(2000))==pdTRUE){
    if ( HIH8120_OK != hih8120_wakeup() )
{
       // Something went wrong
       // Investigate the return code further
}
delay_hal(1000);
if ( HIH8120_OK !=  hih8120_measure() )
{
       // Something went wrong
       // Investigate the return code further
}
hum = hih8120_getHumidityPercent_x10();
_delay_ms(200);
xSemaphoreGive(xTestSemaphore);

    }
}

uint16_t humimpl_getMeasurement(){
	uint16_t returnhum;
    if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(2000))==pdTRUE){
		returnhum = hum;
		xSemaphoreGive(xTestSemaphore);
	}
return returnhum;
}
