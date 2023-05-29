#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "src/handlers/co2Handler/co2Handler.h"
#include "src/handlers/temperturHandler/temperturHandler.h"
#include "src/handlers/HumidityHandler/humidityHandler.h"
#include "src/handlers/servoHandler/servoHandler.h"
#include "src/DataCollection/dataCollection.h"
#include <stdio_driver.h>
#include <serial.h>
#include <lora_driver.h>
#include <status_leds.h>

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);
MessageBufferHandle_t downLinkMessageBufferHandle;

void init_semaphores(void)
{
	if ( xTestSemaphore == NULL )  
	{
		xTestSemaphore = xSemaphoreCreateMutex();  
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) ); 
		}
	}

}

void initialiseSystem()
{
	
	DDRA |= _BV(DDA0) | _BV(DDA7);

	
	stdio_initialise(ser_USART0);
	
	init_semaphores();

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task

	// Initialise the LoRaWAN driver with down-link buffer
	downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);
	lora_driver_initialise(1, downLinkMessageBufferHandle); 

	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
	
	display_7seg_initialise(NULL); 

	dataCollection_init();
	co2_init();
	temp_init();
	humidity_Init();
	servo_handler_init();

}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem();
	vTaskStartScheduler(); 

	while (1)
	{
		
	}
}

