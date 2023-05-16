#include "temperturHandler.h"


#include <stdint.h>


void temp_init(){
    tempimpl_init();
}


uint16_t temp_getMeasurement(){
tempimpl_measure();
tempimpl_getMeasurement();
}


// uint16_t co2_getAvg();

