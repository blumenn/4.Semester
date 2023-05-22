#include <stdint.h>
#include <lora_driver.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <status_leds.h>
#include <queue.h>

lora_driver_payload_t wrapperhandler();

typedef struct {
    char *sensorName;
    SensorStatus status;
    uint16_t data;
} SensorData;

typedef struct
{
    SensorData temp;
    SensorData hum;
    SensorData CO2;
} latestData;
