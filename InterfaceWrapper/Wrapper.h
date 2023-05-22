#include <stdint.h>
#include <lora_driver.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <status_leds.h>
#include "../src/SensorData.h"

lora_driver_payload_t wrapperhandler();

typedef struct {
    int16_t sum;
    int8_t antal;
} measuringSum;
