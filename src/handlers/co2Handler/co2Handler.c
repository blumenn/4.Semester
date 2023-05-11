#include "co2Handler.h"
#include "co2Impl/co2.h"

void co2_init(){
    co2impl_init();
}


uint16_t co2_getMeasurement(){
co2impl_measure();
co2impl_getMeasurement();
}


// uint16_t co2_getAvg();