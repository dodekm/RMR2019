
#include "points.h"
#include "pch.h"


float PointsDistance(Point a, Point b)
{
	return sqrt((a.X - b.X)*(a.X - b.X) + (a.Y - b.Y)*(a.Y - b.Y));
}

float PointAngle(Point p)
{
	return atan2(p.Y, p.X);
}


Point PointFromAngle(float angle, float length)
{
	Point p;
	p.X = length * cos(angle);
	p.Y = length * sin(angle);
	return p;
}



