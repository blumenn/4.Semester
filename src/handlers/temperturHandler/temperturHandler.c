#include "temperturHandler.h"
#include <ATMEGA_FreeRTOS.h>
#include <stdint.h>
#include <queue.h>
#include "../src/implementation/tempImpl/temperaturImpl.h"
extern QueueHandle_t xQueue;

void temp_init(){
    tempimpl_init();
}


uint16_t temp_getMeasurement(){
    
tempimpl_measure();

return tempimpl_getMeasurement();
}


// uint16_t co2_getAvg();

