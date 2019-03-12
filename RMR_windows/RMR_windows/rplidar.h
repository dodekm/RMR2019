
#pragma once


#define lidar_treshold_max 5000
#define lidar_treshold_min 300
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
