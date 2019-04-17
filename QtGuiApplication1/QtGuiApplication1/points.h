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

	RobotPosition operator+(const RobotPosition& other)
	{
		RobotPosition sum;
		sum.alfa=this->alfa + other.alfa;
		sum.coordinates=this->coordinates+other.coordinates;
		return sum;
	}

	RobotPosition operator-(const RobotPosition& other)
	{
		RobotPosition sum;
		sum.alfa = this->alfa - other.alfa;
		sum.coordinates = this->coordinates - other.coordinates;
		return sum;
	}


	RobotPosition operator/(float real)
	{
		RobotPosition div;
		div.alfa=this->alfa / real;

		div.coordinates = Point{ this->coordinates.X / real ,this->coordinates.Y / real };
		return div;
	}


	RobotPosition operator*(float real)
	{

		RobotPosition mul;
		mul.alfa=this->alfa * real;
		mul.coordinates= Point{ this->coordinates.X * real ,this->coordinates.Y * real };
		return mul;
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

	

	bool is_out_of_range(Point actual_position,float treshold)
	{
		return PointsDistance(actual_position, points.front())>treshold && PointsDistance(actual_position, points.back())>treshold;
	
	}

};

