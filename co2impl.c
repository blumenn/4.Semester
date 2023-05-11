

#include <stddef.h>
#include "co2.h"
#include <mh_z19.h>
#include <serial.h>

uint16_t ppm;

void co2CallBack(uint16_t ppmCall);

void co2_sensor_init()
{
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(co2CallBack);
}

void co2CallBack(uint16_t ppmCall)
{
	ppm = ppmCall;
}

void co2_measure()
{
	mh_z19_returnCode_t returnCode = mh_z19_takeMeassuring();
	
	if (returnCode != MHZ19_OK)
	{
		printf("CO2HardwareERROR: %d\n", returnCode);
	}
}


uint16_t co2_getMeasurement()
{
	return ppm;
}