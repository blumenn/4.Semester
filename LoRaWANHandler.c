/*
* loraWANHandler.c
*
* Created: 12/04/2019 10:09:05
*  Author: IHA
*/
#include <stddef.h>
#include <stdio.h>
#include "src/handlers/co2Handler/interface/co2Handler.h"
#include "./InterfaceWrapper/Wrapper.h"



#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <lora_driver.h>
#include <status_leds.h>
#include "../src/handlers/servoHandler/servoHandler.h"
extern SemaphoreHandle_t xTestSemaphore;
// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "7D8AC642ABB372BC"
#define LORA_appKEY "458FC671144070F154BC8984B6051DA7"

void lora_handler_task( void *pvParameters );


static lora_driver_payload_t _uplink_payload;

extern lora_driver_payload_t downlinkPayload;

extern MessageBufferHandle_t downLinkMessageBufferHandle;

uint16_t maxHumSetting; // Max Humidity
int16_t maxTempSetting; // Max Temperature
uint16_t minHumSetting;

uint16_t minTempsetting;

uint16_t maxCo2Setting;

uint16_t minCo2Setting;

void lora_handler_initialise(UBaseType_t lora_handler_task_priority)
{
	xTaskCreate(
	lora_handler_task
	,  "LRHand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  lora_handler_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}


static void _lora_setup(void)
{
	char _out_buf[20];
	lora_driver_returnCode_t rc;
	status_leds_slowBlink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_getRn2483Hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_mapReturnCodeToText(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;
	
	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			status_leds_longPuls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		status_leds_ledOn(led_ST2); // OPTIONAL
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2); // OPTIONAL
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST1); // OPTIONAL

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}

/*-----------------------------------------------------------*/
void lora_handler_task( void *pvParameters )
{
	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);

	lora_driver_flushBuffers(); // get rid of first version string from module after reset!

	_lora_setup();

	wrapper_init();
	

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
//	MessageBufferHandle_t downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2); // Here I make room for two downlink messages in the message buffer
//	lora_driver_initialise(ser_USART1, downLinkMessageBufferHandle); // The parameter is the USART port the RN2483 module is connected to - in this case USART1 - here no message buffer for down-link messages are defined
		
	
	for(;;)
	{
		display_7seg_powerUp();
		maxHumSetting =1+maxHumSetting;
		display_7seg_display((float)maxHumSetting, 1);
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		

		if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(5000))==pdTRUE)
		{
		// Some dummy payload
		
		_uplink_payload = wrapperhandler();
		
		status_leds_shortPuls(led_ST4);  // OPTIONAL
		
		printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));


		// this code must be in the loop of a FreeRTOS task!
		xMessageBufferReceive(downLinkMessageBufferHandle, &downlinkPayload, sizeof(lora_driver_payload_t), 10000);
		//printf("DOWN LINK: from port: %d with %d bytes received!", downlinkPayload.portNo, downlinkPayload.len); // Just for Debug
		if (2 == downlinkPayload.len) // Check that we have got the expected 4 bytes
		{
       // decode the payload into our variales
       maxHumSetting = (downlinkPayload.bytes[0] << 8) + downlinkPayload.bytes[1];
	   
       maxTempSetting = (downlinkPayload.bytes[2] << 8) + downlinkPayload.bytes[3];
	   
		}
		
		if(6 == downlinkPayload.len){
			       maxHumSetting = (downlinkPayload.bytes[0] << 8) + downlinkPayload.bytes[1];
			       
			       minHumSetting = (downlinkPayload.bytes[2] << 8) + downlinkPayload.bytes[3];
				   
				   maxTempSetting = (downlinkPayload.bytes[4] << 8) + downlinkPayload.bytes[5];
				   
				   minTempsetting = (downlinkPayload.bytes[6] << 8) + downlinkPayload.bytes[7];
				   
				   maxCo2Setting = (downlinkPayload.bytes[8] << 8) + downlinkPayload.bytes[9];
				   
				   minCo2Setting = (downlinkPayload.bytes[10] << 8) + downlinkPayload.bytes[11];
				   setConfig(maxHumSetting,minHumSetting,maxTempSetting,minTempsetting,maxCo2Setting,minCo2Setting);
				   
		}
				xSemaphoreGive(xTestSemaphore);
		}
	
		
	}
}
