#include "servoHandler.h"
#include "implementation/servo/servoImpl.h"
#include "handlers/temperturHandler/temperturHandler.h"
#include "handlers/co2Handler/interface/co2Handler.h"
#include "handlers/HumidityHandler/humidityHandler.h"
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>
static config configuration;
static SemaphoreHandle_t servoTestSemaphore;
extern QueueHandle_t xQueue;

void servo_handler_init(void){
servo_init();
	if ( servoTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		servoTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( servoTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( servoTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}
return;
}
void servo_set_config(uint16_t maxHumSetting,
    uint16_t minHumSetting,
    uint16_t maxTempSetting,
    uint16_t minTempsetting,
    uint16_t maxCo2Setting,
    uint16_t minCo2Setting){
         if(xSemaphoreTake(servoTestSemaphore,pdMS_TO_TICKS(200))==pdTRUE){
        
            configuration.maxHumSetting = maxHumSetting;
configuration.minHumSetting = minHumSetting;
configuration.maxTempSetting = maxTempSetting;
configuration.minTempsetting = minTempsetting;
configuration.maxCo2Setting = maxCo2Setting;
configuration.minCo2Setting = minCo2Setting;
xSemaphoreGive(servoTestSemaphore);
        }
        

}

void servo_measuring(void){
    if(xSemaphoreTake(servoTestSemaphore,pdMS_TO_TICKS(200))==pdTRUE){
        {
    uint16_t temp = temp_getMeasurement();
    uint16_t hum = hum_getMeasurement();
   uint16_t co2 = co2_getMeasurement();
    if (configuration.maxHumSetting<hum)
    {
        servoOpenWindow();
        xSemaphoreGive(servoTestSemaphore);
        return ;
    }
    if (configuration.maxCo2Setting < co2)
    {
        servoOpenWindow();
        xSemaphoreGive(servoTestSemaphore);
        return ;
    }
    if (configuration.maxTempSetting<temp)
    {
        servoOpenWindow();
        xSemaphoreGive(servoTestSemaphore);
        return ;
    }
    if (configuration.minCo2Setting>co2)
    {
        servoCloseWindow();
        xSemaphoreGive(servoTestSemaphore);
		return;
    }
    if(configuration.minHumSetting> hum){
        servoCloseWindow();
        xSemaphoreGive(servoTestSemaphore);
		return;
    }
    if (configuration.minTempsetting> temp)
    {
        xSemaphoreGive(servoTestSemaphore);
		servoCloseWindow();
        return;
    }
    xSemaphoreGive(servoTestSemaphore);
        }
    return;
    
    
    
}
}