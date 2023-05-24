#include "temperaturimpl.h"
#include "delay_hal.h"
#include <ATMEGA_FreeRTOS.h>
#include <stdint.h>
#include <hih8120.h>
#include <semphr.h>






static uint16_t temp = 0;
extern SemaphoreHandle_t xTestSemaphore;


void tempimpl_init(){
     if ( HIH8120_OK == hih8120_initialise() )
{
       // Driver initialised OK
       // Always check what hih8120_initialise() returns
}
}
void tempimpl_measure(){
    if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(200))==pdTRUE){
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
temp = hih8120_getTemperature_x10();
xSemaphoreGive(xTestSemaphore);
    }
}

uint16_t tempimpl_getMeasurement(){
	uint16_t returntemp = 0;
    if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(200))==pdTRUE){
		returntemp = temp;
		xSemaphoreGive(xTestSemaphore);
	}
return returntemp;
}

