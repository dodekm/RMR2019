
#include "points.h"

float PointsDistance(Point_ a, Point_ b)
{
	return sqrt((a.X - b.X)*(a.X - b.X) + (a.Y - b.Y)*(a.Y - b.Y));
}
float PointLength(Point_ a)
{
	return sqrt((a.X)*(a.X) + (a.Y)*(a.Y));
}

float PointAngle(Point_ p)
{
	return atan2(p.Y, p.X);
}


Point_ polar2point(float angle, float length)
{
	Point_ p;
	p.X = length * cos(angle);
	p.Y = length * sin(angle);
	return p;
}



