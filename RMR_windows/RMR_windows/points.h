#pragma once

typedef struct Point
{
	float X = 0.0;
	float Y = 0.0;
}Point;


float PointsDistance(Point a, Point b);


class Position
{
public:
	Point coordinates;
	float alfa;

	Position()
	{
		alfa = 0.0;
		coordinates = { 0.0,0.0 };
	}

	Position(float X, float Y, float alfa)
	{
		coordinates.X = X;
		coordinates.Y = Y;
		this->alfa = alfa;
	}

	Position(float X, float Y)
	{
		coordinates.X = X;
		coordinates.Y = Y;
		alfa = 0.0;
	}

	~Position()
	{
	
	}

};