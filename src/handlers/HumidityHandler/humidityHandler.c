#include <stdint.h>
//#include "./src/implementation/humidityImpl/humidityImpl.h"
//#include "./src/handlers/HumidityHandler/interface/humidityHandler.h"
#include "humidityHandler.h"

uint16_t hum_getMeasurement(){
	humimpl_measure();
    return humimpl_getMeasurement();
}

