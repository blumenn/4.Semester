#pragma once
#include <stdint.h>
#include <lora_driver.h>
#include "../src/SensorData.h"

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

lora_driver_payload_t wrapperhandler();
void dataCollection_init();
latestData get_latestData();

