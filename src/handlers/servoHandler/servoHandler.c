#include "servoHandler.h"
#include "../implementation/servo/servoImpl.h"
#include "../handlers/temperturHandler/temperturHandler.h"
#include "../handlers/co2Handler/interface/co2Handler.h"
#include "../handlers/HumidityHandler/humidityHandler.h"
static config configuration;

void servo_handler_init(void){
servo_init();
return;
}
void servo_set_config(uint16_t maxHumSetting,
    uint16_t minHumSetting,
    uint16_t maxTempSetting,
    uint16_t minTempsetting,
    uint16_t maxCo2Setting,
    uint16_t minCo2Setting){
configuration.maxHumSetting = maxHumSetting;
configuration.minHumSetting = minHumSetting;
configuration.maxTempSetting = maxTempSetting;
configuration.minTempsetting = minTempsetting;
configuration.maxCo2Setting = maxCo2Setting;
configuration.minCo2Setting = minCo2Setting;
}
void servo_measuring(void){
    uint16_t temp = temp_getMeasurement();
    uint16_t hum = hum_getMeasurement();
    uint16_t co2 = co2_getMeasurement();
    if (configuration.maxHumSetting<hum)
    {
        servoOpenWindow();
        return ;
    }
    if (configuration.maxCo2Setting < co2)
    {
        servoOpenWindow();
        return ;
    }
    if (configuration.maxTempSetting<temp)
    {
        servoOpenWindow();
        return ;
    }
    if (configuration.minCo2Setting>co2)
    {
        return;
    }
    if(configuration.minHumSetting> hum){
        return;
    }
    if (configuration.minTempsetting> temp)
    {
        return;
    }
    servoCloseWindow();
    return;
    
    
    
}
