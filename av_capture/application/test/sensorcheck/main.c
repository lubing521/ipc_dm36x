#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum enumSensorType
{
    AR0130    = 0,
    AR0331,
    MT9M034,
    MT9P031,
    AP0101,
    IMX122,
    AR0330,
    OV9712,
    SENSOR_UNKNOWN,
    SENSOR_NONE
} eSensorType;

int main(int argc, char **argv)
{
    CheckSensor();
    eSensorType sensorId = GetSensorId();
    switch (sensorId)
    {
    case OV9712:
        printf("Sensor is OV9712\n");
        break;
    case AR0130:
        printf("Sensor is AR0130\n");
        break;
    case MT9M034:
        printf("Sensor is MT9M034\n");
        break;
    case IMX122:
        printf("Sensor is IMX122\n");
        break;
    case AR0330:
        printf("Sensor is AR0330\n");
        break;
    case AR0331:
        printf("Sensor is AR0331\n");
        break;
    case MT9P031:
        printf("Sensor is MT9P031\n");
        break;
    default:
        printf("Sensor is error\n");
        break;
    }

    return sensorId;
}

