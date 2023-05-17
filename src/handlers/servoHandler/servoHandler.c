#include "servoHandler.h"
#include "../src/handlers/co2Handler/interface/co2Handler.h"
#include "../src/handlers/HumidityHandler/interface/humidityHandler.h"
#include "../src/handlers/temperturHandler/interface/temperturHandler.h"
#include "../src/implementation/servo/servoImpl.h"

static config configuration;

void servoHandlerInit(){
    servo_init();
}

void setConfig( uint16_t maxHumSetting,
    uint16_t minHumSetting,
    uint16_t maxTempSetting,
    uint16_t minTempSetting,
    uint16_t maxCO2,
    uint16_t minCO2 ){
    configuration.maxCO2 = maxCO2;
    configuration.minCO2 = minCO2;
    configuration.maxHumSetting = maxHumSetting;
    configuration.minHumSetting = minHumSetting;
    configuration.maxTempSetting = maxTempSetting;
    configuration.minTempSetting = minTempSetting;
}
void servoMeasuring(void){
    uint16_t co2 = co2_getMeasurement();
    uint16_t temp = temp_getMeasurement();
    uint16_t hum = hum_getMeasurement();
    if(configuration.maxCO2<co2 || configuration.maxTempSetting<temp||configuration.maxTempSetting<hum)
    {
        servoOpenWindow();
    }else if(configuration.minCO2>co2 || configuration.minTempSetting>temp||configuration.minHumSetting
    >hum){
        servoCloseWindow();
    }
    
}

