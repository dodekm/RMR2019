#pragma once
#include "points.h"
#include "rplidar.h"
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <new> 
#include <queue>
#include <vector>

#define floodfill_priority_X 0
#define floodfill_priority_Y 1

typedef int  floodfill_priority;

typedef enum
{
	cell_free,
	cell_obstacle,
	cell_finish,
	cell_start = -1,
	cell_path = -2,
	cell_breakpoint=-3
	
}cell_content;


class Matrix_position
{
	public:
		int X, Y;

		Matrix_position()
		{}
		~Matrix_position()
		{}
		Matrix_position(int X,int Y)
		{
			this->X = X;
			this->Y = Y;
		}

	bool operator==(const Matrix_position& other)
	{
		return (X==other.X&&Y==other.Y);
	}

	bool operator!=(const Matrix_position& other)
	{
		return !((*this)==other);
	}

	void operator=(const Matrix_position& other)
	{
		X = other.X;
		Y = other.Y;
	}
	

	Matrix_position operator +(const Matrix_position& other)
	{
		return Matrix_position{ X + other.X,Y + other.Y };
	}

	Matrix_position operator -(const Matrix_position& other)
	{
		return Matrix_position{ X - other.X,Y - other.Y };
	}
	private:
	

};


Point lidar_measure_2_point(LaserData lidar_measurement, RobotPosition robot_position);
float deg2rad(float);
float rad2deg(float);


class Mapa
{

public:
	Mapa()
	{
	
	}
	
	Mapa(int rows,int cols, float  x_low,float x_high,float y_low,float y_high,std::string filename)
	{
		cells_data = new int*[rows];
		for (int i = 0; i < rows; ++i) 
		{
			cells_data[i] = new int[cols];
			for (int j = 0; j < cols; j++)
				cells_data[i][j] = cell_free;
		}
		this->cols = cols;
		this->rows = rows;
		this->x_lim[0] = x_low;
		this->x_lim[1] = x_high;
		this->y_lim[0] = y_low;
		this->y_lim[1] = y_high;

		if (filename.empty())
			return;
		
		loadMap(filename);
		
		//saveMap(filename);
	}


	void  operator=(const Mapa& source)
	{
		
		cols = source.cols;
		rows = source.rows;
		x_lim[0] = source.x_lim[0];
		x_lim[1] = source.x_lim[1];
		y_lim[0] = source.y_lim[0];
		y_lim[1] = source.y_lim[1];

		cells_data = new int*[rows];
		for (int i = 0; i < rows; ++i)
		{
			cells_data[i] = new int[cols];
			for (int j = 0; j < cols; j++)
				cells_data[i][j] = source.cells_data[i][j];
		}
		
	}

	auto& operator[](Matrix_position position)
	{
		return (cells_data[position.Y][position.X]);
	}

	
	Mapa(const Mapa& source)
	{
	
		cols = source.cols;
		rows = source.rows;
		x_lim[0] = source.x_lim[0];
		x_lim[1] = source.x_lim[1];
		y_lim[0] = source.y_lim[0];
		y_lim[1] = source.y_lim[1];

		cells_data = new int*[rows];
		for (int i = 0; i < rows; ++i)
		{
			cells_data[i] = new int[cols];
			for (int j = 0; j < cols; j++)
				cells_data[i][j] = source.cells_data[i][j];
		}

	}

	~Mapa()
	{
		for (int i = 0; i < rows; ++i) {
			delete[] cells_data[i];
		}
		delete[] cells_data;
	
	}

	void FloodFill_fill(Point start, Point target,bool diagonal);
	void FloodFill_find_path(Point start, Point target, floodfill_priority priority, std::queue <RobotPosition> path);

	int assert_matrix_indices(Matrix_position XY);
	int addObstacle(Point P);

	void saveMap(std::string filename);

	void loadMap(std::string filename);

private:

	int cols, rows;
	float  x_lim[2], y_lim[2];
	int** cells_data;
	

	Matrix_position point2indices(Point P)
	{
		Matrix_position XY;
		XY.X = (int)round(cols * (P.X - x_lim[0]) / (x_lim[1] - x_lim[0]));
		XY.Y = (int)round(rows * (P.Y - y_lim[0]) / (y_lim[1] - y_lim[0]));
		return XY;
	}


	Point indices2point( Matrix_position XY)
	{
		Point P;
		P.X = (x_lim[1] - x_lim[0])*XY.X / cols + x_lim[0];
		P.Y = (y_lim[1] - y_lim[0])*XY.Y / rows + y_lim[0];
		return P;
	}
	

};

