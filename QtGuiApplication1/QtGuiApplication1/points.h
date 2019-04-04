#pragma once

#include <cmath>

template <typename T>int sign(T x)
{
	return (((x) < 0) ? -1 : ((x) > 0));
}

typedef struct Point
{
	float X = 0.0;
	float Y = 0.0;

	Point operator +(const Point& other)
	{
		return Point{X+other.X,Y+other.Y};
	}

	Point operator-(const Point& other)
	{
		return Point{ X - other.X,Y - other.Y };
	}

}Point;


float PointsDistance(Point a, Point b);
float point2angle(Point p);
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

	RobotPosition(const RobotPosition& other)
	{
		this->alfa = other.alfa;
		this->coordinates = other.coordinates;
	}
	
	RobotPosition& operator=(const RobotPosition& other)
	{
		this->alfa = other.alfa;
		this->coordinates = other.coordinates;
		return *this;
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

	RobotPosition(Point P)
	{
		coordinates.X = P.X;
		coordinates.Y = P.Y;
		alfa = 0;
	}

	~RobotPosition()
	{
	
	}

};

