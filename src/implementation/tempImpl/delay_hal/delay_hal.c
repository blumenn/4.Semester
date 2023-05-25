#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include "delay_hal.h"

void delay_hal(uint16_t ms){

    _delay_ms(ms);
}
