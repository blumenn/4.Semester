#include <stdint.h>

void create_co2handler_task(){
    xTaskCreate(
    co2_handler_task, 
    "co2_handler_task",
    1000,
    NULL,
    1,
    NULL);

}

void co2_init(){
    co2impl_init();
}


uint16_t co2_getMeasurement(){
co2impl_measure();
co2impl_getMeasurement();
}


// uint16_t co2_getAvg();