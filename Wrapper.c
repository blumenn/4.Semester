#include <stdint.h>
#include "./src/handlers/co2Handler/interface/co2Handler.h"
#include "./src/handlers/HumidityHandler/interface/humidityHandler.h"
#include "./src/handlers/temperturHandler/interface/temperturHandler.h"

static lora_driver_payload_t _uplink_payload;

void wrapper_init(){
	_uplink_payload.len = 6;
	_uplink_payload.portNo = 2;
}

 lora_driver_payload_t wrapperhandler()
{
lora_driver_payload_t _uplink_payload;
uint16_t co2_ppm = co2_getMeasurement();
uint16_t temp = temp_getMeasurement();
uint16_t hum = hum_getMeasurement();

	_uplink_payload.bytes[0] = hum >> 8;
	_uplink_payload.bytes[1] = hum & 0xFF;
	_uplink_payload.bytes[2] = temp >> 8;
	_uplink_payload.bytes[3] = temp & 0xFF;
	_uplink_payload.bytes[4] = co2_ppm >> 8;
	_uplink_payload.bytes[5] = co2_ppm & 0xFF;

	
}
