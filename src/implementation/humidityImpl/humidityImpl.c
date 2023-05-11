#include <stdint.h>
#include <hih8120.h>

uint16_t temp;
extern SemaphoreHandle_t xTestSemaphore;


void tempimpl_init(){
     if ( HIH8120_OK == hih8120_initialise() )
{
       // Driver initialised OK
       // Always check what hih8120_initialise() returns
}
}
void tempimpl_measure(){
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
temp = hih8120_getTemperature();
xSemaphoreGive(xTestSemaphore);
    }
}
uint16_t tempimpl_getMeasurement(){
    
return temp;
}