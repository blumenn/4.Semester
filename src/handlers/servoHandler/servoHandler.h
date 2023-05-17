#pragma once
#include <stdint.h>
struct config
{
    uint16_t maxHumSetting;
    uint16_t minHumSetting;
    uint16_t maxTempSetting;
    uint16_t minTempsetting;
    uint16_t maxCo2Setting;
    uint16_t minCo2Settingv;
} config;



void servo_handler_init(void);
void servo_ser_config(void);
void servo_measuring(void);