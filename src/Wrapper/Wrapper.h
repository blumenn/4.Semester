#include <stdint.h>
#include <lora_driver.h>

#include "../src/SensorData.h"
lora_driver_payload_t wrapperhandler();

typedef struct
{
    SensorData temp;
    SensorData hum;
    SensorData co2;
} latestData;


typedef struct {
    uint16_t sum;
    uint8_t antal;
} measuringSum;

latestData get_latestData();

