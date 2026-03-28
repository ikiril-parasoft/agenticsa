#include <stdint.h>
#include <errno.h>
#include <stdlib.h>  
#include <string.h>  

volatile uint16_t SENSOR_DATA = 0;
volatile uint16_t MOTOR_SPEED = 0;

volatile uint16_t SENSOR_HIGH_THRESHOLD = 500;
volatile uint16_t SENSOR_LOW_THRESHOLD = 100;
volatile uint16_t DEFAULT_VALUE = 10;
volatile uint16_t MULTIPLIER = 2;

static int parseCommandValue(const char* const arg, int* const out)
{
    if ((arg == NULL) || (out == NULL)) {
        return -1;
    }

    char* endPtr = NULL;
    errno = 0;
    long const parsed = strtol(arg, &endPtr, 10);

    if ((endPtr == arg) || (*endPtr != '\0') || (errno == ERANGE) || (parsed < 0) || (parsed > UINT16_MAX)) {
        return -1;
    }

    *out = (int)parsed;
    return 0;
}


uint16_t readSensor()
{
    return SENSOR_DATA; 
}

void setMotorSpeed(uint16_t const speed)
{
    MOTOR_SPEED = speed;
}

int computeControl(int const sensor, int const divisor)
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

int processCommand(const char* const cmd, const char* const arg)
{
    if (strcmp(cmd, "SET") == 0) {
        int parsedVal = 0;
        if (parseCommandValue(arg, &parsedVal) != 0) {
            return -1;
        }
        int const val = parsedVal;
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
    uint16_t const sensor = readSensor();
    int const control = computeControl(sensor, 0); 
    return processCommand(cmd, arg) + control;
}