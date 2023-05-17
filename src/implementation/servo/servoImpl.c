#include "servoImpl.h"

void servo_init(){
    rc_servo_initialise();
}

void servoOpenWindow(){
    rc_servo_setPosition(1, 100);
}

void servoCloseWindow(){
    rc_servo_setPosition(1, -100);
}