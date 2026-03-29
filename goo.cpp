#include <stdint.h>
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>
#include <limits.h>

volatile uint16_t SENSOR_DATA = 0;
volatile uint16_t MOTOR_SPEED = 0;

volatile uint16_t SENSOR_HIGH_THRESHOLD = 500;
volatile uint16_t SENSOR_LOW_THRESHOLD = 100;
volatile uint16_t DEFAULT_VALUE = 10;
volatile uint16_t MULTIPLIER = 2;


uint16_t readSensor()
{
    return SENSOR_DATA; 
}

void setMotorSpeed(const uint16_t speed)
{
    MOTOR_SPEED = speed;
}

int computeControl(const int sensor, const int divisor)
{
    const int value = 0;

    if (sensor > SENSOR_HIGH_THRESHOLD) {
        if (divisor == 0) {
            return DEFAULT_VALUE;
        }
        return sensor / divisor;
    } else if (sensor > SENSOR_LOW_THRESHOLD) {
        return sensor * MULTIPLIER;
    } else {
        return DEFAULT_VALUE;
    }
    return value;
}

int processCommand(const char* const cmd, const char* const arg)
{
    if (cmd == NULL) {
        return -1;
    }

    if (strcmp(cmd, "SET") == 0) {
        char* endPtr = NULL;
        long parsed = 0;

        if (arg == NULL) {
            return -1;
        }

        errno = 0;
        parsed = strtol(arg, &endPtr, 10);
        if ((endPtr == arg) || (*endPtr != '\0') || (errno == ERANGE) || (parsed < 0) || (parsed > UINT16_MAX)) {
            return -1;
        }

        const int val = (int)parsed;
        setMotorSpeed((uint16_t)val);
        return val;
    }

    if (strcmp(cmd, "READ") == 0) {
        return readSensor();
    }
    return -1; 
}

int processor(const char* const cmd, const char* const arg)
{
    const uint16_t sensor = readSensor();
    const int control = computeControl(sensor, 0); 
    return processCommand(cmd, arg) + control;
}
