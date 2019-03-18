#pragma once
#include <cmath>

typedef struct Point_
{
	float X = 0.0;
	float Y = 0.0;

	Point_ operator +(const Point_& other)
	{
		return Point_{X+other.X,Y+other.Y};
	}

	Point_ operator-(const Point_& other)
	{
		return Point_{ X - other.X,Y - other.Y };
	}

}Point_;


float PointsDistance(Point_ a, Point_ b);
float PointAngle(Point_ p);
float PointLength(Point_ a);
Point_ polar2point(float angle, float length);

class RobotPosition
{
public:
	Point_ coordinates;
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

	RobotPosition(Point_ P)
	{
		coordinates.X = P.X;
		coordinates.Y = P.Y;
		alfa = 0;
	}

	~RobotPosition()
	{
	
	}

};