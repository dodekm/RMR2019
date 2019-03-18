#include "pch.h"
#include "rplidar.h"


int lidar_check_measure(LaserData measure)
{
	if (measure.scanQuality < lidar_treshold_quality)
	{
		return 0;
	}
	if (measure.scanDistance<lidar_treshold_min || measure.scanDistance>lidar_treshold_max)
	{
		return 0;
	}
	else
		return 1;
}



