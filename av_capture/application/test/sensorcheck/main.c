#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int status = CheckSensor();
    int sensorid = GetSensorId();
    if (status != 0)
    {
        printf("Can not identify sensor!!!\n");
        return -1;
    }
    else
    {
        printf("sensorId = %d\n", sensorid);
    }

    return sensorid;
}

