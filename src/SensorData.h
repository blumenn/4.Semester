#pragma once

#include <stdint.h>

typedef enum {
    SENSOR_STATUS_OK,
    SENSOR_STATUS_ERROR
} SensorStatus;

typedef struct {
    char *sensorName;
    SensorStatus status;
    uint16_t data;
} SensorData;
