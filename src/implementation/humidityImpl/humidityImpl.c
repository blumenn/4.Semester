#include "humidityImpl.h"
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include "../tempImpl/delay_hal/delay_hal.h"

static uint16_t hum;
extern SemaphoreHandle_t xTestSemaphore;

void humimpl_measure(){
    if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(2000))==pdTRUE){
        if ( HIH8120_OK != hih8120_wakeup() )
        {
        }
        delay_hal(100);
        if ( HIH8120_OK !=  hih8120_measure() )
        {
        }
        hum = hih8120_getHumidityPercent_x10();
        delay_hal(20);
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
