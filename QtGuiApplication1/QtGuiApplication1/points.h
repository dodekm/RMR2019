#pragma once

#include <cmath>
#include <list>



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
float Point_angle(Point p);
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

	RobotPosition& operator+(const RobotPosition& other)
	{
		this->alfa += other.alfa;
		this->coordinates = this->coordinates+other.coordinates;
		return *this;
	}

	RobotPosition& operator/(float real)
	{
		this->alfa /= real;
		this->coordinates = Point{ this->coordinates.X / real ,this->coordinates.Y / real };
		return *this;
	}


	RobotPosition& operator*(float real)
	{
		this->alfa *= real;
		this->coordinates = Point{ this->coordinates.X * real ,this->coordinates.Y * real };
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


class obstacle
{

private:
	

public:
	std::list<Point>points;

	std::list<Point> get_edges()
	{
		std::list<Point> l;
		l.push_back(*points.begin());
		l.push_back(*points.end());
		return l;
	}

	bool is_out_of_range(Point actual_position,float treshold)
	{
		return PointsDistance(actual_position, *points.begin())>treshold && PointsDistance(actual_position, *points.end())>treshold;
	
	}

};

