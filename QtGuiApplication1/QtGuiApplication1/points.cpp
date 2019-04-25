
#include "points.h"


float PointsDistance(Point a, Point b)
{
	return sqrt((a.X - b.X)*(a.X - b.X) + (a.Y - b.Y)*(a.Y - b.Y));
}
float PointLength(Point a)
{
	return sqrt((a.X)*(a.X) + (a.Y)*(a.Y));
}

float Point_angle(Point p)
{
	return atan2(p.Y, p.X);
}


Point polar2point(float angle, float length)
{
	Point p;
	p.X = length * cos(angle);
	p.Y = length * sin(angle);
	return p;
}


int is_triangle_sharp(Point A, Point B, Point C)
{
	
	float AB = PointsDistance(A,B);
	float AC = PointsDistance(A,C);
	float BC = PointsDistance(B,C);
	return (AC*AC + BC * BC) < AB*AB;
		
}

