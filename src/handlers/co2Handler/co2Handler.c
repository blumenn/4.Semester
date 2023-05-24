#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include <queue.h>
extern QueueHandle_t xQueue;
void co2_init(){
    co2impl_init();
}


uint16_t co2_getMeasurement(){
co2impl_measure();
return co2impl_getMeasurement();
}


// uint16_t co2_getAvg();