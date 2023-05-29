#include "temperaturImpl.h"
#include "delay_hal/delay_hal.h"
#include <ATMEGA_FreeRTOS.h>
#include <hih8120.h>
#include <semphr.h>

static uint16_t temp = 0;
extern SemaphoreHandle_t xTestSemaphore;

void tempimpl_init(){
     if ( HIH8120_OK == hih8120_initialise() )
       {
       }
}

void tempimpl_measure(){
    if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(2000))==pdTRUE){
              if ( HIH8120_OK != hih8120_wakeup() )
              {
              }
              delay_hal(100);
              if ( HIH8120_OK !=  hih8120_measure() )
              {
              }
              temp = hih8120_getTemperature_x10();
              delay_hal(20);
              xSemaphoreGive(xTestSemaphore);
       }
}

uint16_t tempimpl_getMeasurement(){
       if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(200))==pdTRUE){
              uint16_t returntemp = 0;
		returntemp = temp;
		xSemaphoreGive(xTestSemaphore);
		return returntemp;
	}
       return 0;
}