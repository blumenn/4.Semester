#include <stdint.h>
#include <stdint.h>
#include <lora_driver.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <status_leds.h>

lora_driver_payload_t wrapperhandler();