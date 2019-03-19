
#pragma once


#define lidar_treshold_max 2000
#define lidar_treshold_min 200
#define lidar_treshold_quality 10

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

int lidar_check_measure(LaserData);
