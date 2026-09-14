#include <stdint.h>
#include <stdlib.h>  
#include <string.h>  

static int parseU16(const char* text, uint16_t* out)
{
    char* endptr = NULL;
    long parsed = 0L;

    if (text == NULL) {
        return -1;
    }

    if (out == NULL) { return -1; } // parasoft-cov-suppress ALL "Defensive check: output pointer is always valid at current call sites"

    parsed = strtol(text, &endptr, 10);
    if ((endptr == text) || (*endptr != '\0')) {
        return -1;
    }

    if ((parsed < 0L) || (parsed > UINT16_MAX)) {
        return -1;
    }

    *out = (uint16_t)parsed;
    return 0;
}

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

void setMotorSpeed(uint16_t speed)
{
    MOTOR_SPEED = speed;
}

int computeControl(int sensor, int divisor)
{
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
}

int processCommand(const char* cmd, const char* arg)
{
    if (strcmp(cmd, "SET") == 0) {
        uint16_t parsedValue = 0U;
        if (parseU16(arg, &parsedValue) != 0) {
            return -1;
        }

        const int val = (int)parsedValue;
        setMotorSpeed(parsedValue);
        return val;
    }

    if (strcmp(cmd, "READ") == 0) {
        return readSensor();
    }
    return -1; 
}

int processor(const char* cmd, const char* arg)
{
    const uint16_t sensor = readSensor();
    const int control = computeControl(sensor, 0); 
    return processCommand(cmd, arg) + control;
}