#pragma once
#include <stdint.h>
typedef struct config
{
    uint16_t maxHumSetting;
    uint16_t minHumSetting;
    uint16_t maxTempSetting;
    uint16_t minTempsetting;
    uint16_t maxCo2Setting;
    uint16_t minCo2Setting;
} config;



void servo_handler_init(void);
void servo_set_config(uint16_t maxHumSetting,
    uint16_t minHumSetting,
    uint16_t maxTempSetting,
    uint16_t minTempsetting,
    uint16_t maxCo2Setting,
    uint16_t minCo2Setting);
void servo_measuring(void);
void open();