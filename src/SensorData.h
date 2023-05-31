#pragma once

#include <stdint.h>

typedef enum {
    SENSOR_STATUS_OK,
    SENSOR_STATUS_ERROR
} SensorStatus;

typedef enum {
	Co2Sensor,
	hum,
	temp
	}Sensor;
typedef struct {
    Sensor sensorName;
    SensorStatus status;
    uint16_t data;
} SensorData;
