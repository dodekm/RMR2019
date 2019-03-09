#pragma once
#include <cmath>

typedef struct Point
{
	float X = 0.0;
	float Y = 0.0;
}Point;


float PointsDistance(Point a, Point b);
float PointAngle(Point p);
float PointLength(Point a);
Point polar2point(float angle, float length);

class RobotPosition
{
public:
	Point coordinates;
	float alfa;

	RobotPosition()
	{
		alfa = 0.0;
		coordinates = { 0.0,0.0 };
	}

	RobotPosition(float X, float Y, float alfa)
	{
		coordinates.X = X;
		coordinates.Y = Y;
		this->alfa = alfa;
	}

	RobotPosition(float X, float Y)
	{
		coordinates.X = X;
		coordinates.Y = Y;
		alfa = 0;
	}

	~RobotPosition()
	{
	
	}

};