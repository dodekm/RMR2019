
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "iostream"
#include "fcntl.h"
#include<string.h>
#include <errno.h>
#include<sys/types.h>


typedef struct
{
    int scanQuality;
    double scanAngle;
    double scanDistance;
}LaserData;

typedef struct
{
    int numberOfScans;
    LaserData Data[1000];
}LaserMeasurement;
