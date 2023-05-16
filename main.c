/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/
#include <display_7seg.h>
#include <stdio.h>
#include <avr/io.h>
#include <hih8120.h>
#include <util/delay.h>
#include <mh_z19.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "src/handlers/co2Handler/interface/co2Handler.h"
#include "src/handlers/temperturHandler/temperturHandler.h"



#include <stdio_driver.h>
#include <serial.h>


 // Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

// define two Tasks
void task1( void *pvParameters );
void task2( void *pvParameters );

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);
lora_driver_payload_t downlinkPayload;
MessageBufferHandle_t downLinkMessageBufferHandle;

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}

	xTaskCreate(
	task1
	,  "Task1"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );

	xTaskCreate(
	task2
	,  "Task2"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

/*-----------------------------------------------------------*/

	uint16_t ppm;
	
	void Co2CallBack(uint16_t ppmCallback)
	{
		ppm = ppmCallback;
	}

	void co2_taskRun()
	{

		mh_z19_returnCode_t rc;

		
		rc = mh_z19_takeMeassuring();
		if (rc != MHZ19_OK)
		{
			// Something went wrong
		}
	}

	uint16_t co2_getPpm(){
		return ppm;
	}
void task1( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500/portTICK_PERIOD_MS; // 500 ms

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		puts("Task1"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		PORTA ^= _BV(PA0);

		co2_taskRun();
	}


}

/*-----------------------------------------------------------*/
void task2( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000/portTICK_PERIOD_MS; // 1000 ms

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		puts("Task2"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		PORTA ^= _BV(PA7);
		
/*		 
if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(200))==pdTRUE){
if ( HIH8120_OK != hih8120_wakeup() )
{
       // Something went wrong
       // Investigate the return code further
}
_delay_ms(1000);
if ( HIH8120_OK !=  hih8120_measure() )
{
       // Something went wrong
       // Investigate the return code further
}
_delay_ms(1000);
float humidity = 0.0;
float temperature = 0.0;
humidity = hih8120_getHumidity();
temperature = hih8120_getTemperature();

	// Power up the display
	//display_7seg_powerUp();
	//display_7seg_display(humidity, 1);
	xSemaphoreGive(xTestSemaphore);
	}
*/	_delay_ms(10);

}
}

/*-----------------------------------------------------------*/

void initialiseSystem()
{
	
	DDRA |= _BV(DDA0) | _BV(DDA7);

	
	stdio_initialise(ser_USART0);
	
	create_tasks_and_semaphores();

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
	downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2); // Here I make room for two downlink messages in the message buffer
	lora_driver_initialise(1, downLinkMessageBufferHandle); // The parameter is the USART port the RN2483 module is connected to - in this case USART1 - here no message buffer for down-link messages are defined
		
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
	
	display_7seg_initialise(NULL); 

_delay_ms(1000);
co2_init();
temp_init();

}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	while (1)
	{
	}
}

