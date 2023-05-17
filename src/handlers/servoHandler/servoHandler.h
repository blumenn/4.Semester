#include <stdint.h>

typedef struct config
{
			    
    uint16_t maxHumSetting;
    uint16_t minHumSetting;
    uint16_t maxTempSetting;
    uint16_t minTempSetting;
    uint16_t maxCO2;
    uint16_t minCO2;
} config;
void servoHandlerInit(void);

void setConfig( uint16_t maxHumSetting,
    uint16_t minHumSetting,
    uint16_t maxTempSetting,
    uint16_t minTempSetting,
    uint16_t maxCO2,
    uint16_t minCO2 );
void servoMeasuring(void);


