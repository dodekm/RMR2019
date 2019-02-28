#include "map.h"
#include "pch.h"
Point homogen_transformation(LaserData lidar_measurement, Position robot_position)
{
	Point P;
	P.X = robot_position.coordinates.X + lidar_measurement.scanDistance*cos(lidar_measurement.scanAngle + robot_position.alfa);
	P.Y = robot_position.coordinates.Y + lidar_measurement.scanDistance*sin(lidar_measurement.scanAngle + robot_position.alfa);

	return P;
}