#pragma once
#define _USE_MATH_DEFINES

#include "points.h"
#include "rplidar.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <new>  
#include <queue>
#include <vector>
#include <cmath>

#include <QMetaType>

#define floodfill_priority_X 1
#define floodfill_priority_Y 2
#define floodfill_priority_diag 3

typedef int  floodfill_priority;

typedef enum
{
	cell_free,
	cell_obstacle,
	cell_finish,
	cell_start = -1,
	cell_path = -2,
	cell_breakpoint=-3,
	cell_robot=-4
	
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
	
	Mapa(int rows,int cols, float  x_low,float x_high,float y_low,float y_high,std::string filename="")
	{
		
		this->cols = cols;
		this->rows = rows;
		this->x_lim[0] = x_low;
		this->x_lim[1] = x_high;
		this->y_lim[0] = y_low;
		this->y_lim[1] = y_high;

		allocate();

		if (!filename.empty())
		loadMap(filename);
		
	}


	void  operator=(const Mapa& source)
	{
		deallocate();
		cols = source.cols;
		rows = source.rows;
		x_lim[0] = source.x_lim[0];
		x_lim[1] = source.x_lim[1];
		y_lim[0] = source.y_lim[0];
		y_lim[1] = source.y_lim[1];
		allocate();

		for (int i = 0; i < rows; ++i)
		{

			for (int j = 0; j < cols; j++)
			{
				cells_data[i][j] = source.cells_data[i][j];
			}
		}
		
	}

	auto& operator[](Matrix_position position)
	{
		if (cells_data != NULL && assert_matrix_indices(position))
		{
			return (cells_data[position.Y][position.X]);
		}	
		
	}

	int get_rows() { return rows; }
	int get_cols() { return cols; }

	Mapa(const Mapa& source,bool copy=true)
	{
	
		cols = source.cols;
		rows = source.rows;
		x_lim[0] = source.x_lim[0];
		x_lim[1] = source.x_lim[1];
		y_lim[0] = source.y_lim[0];
		y_lim[1] = source.y_lim[1];

		allocate();

		for (int i = 0; i < rows; ++i)
		{
			
			for (int j = 0; j < cols; j++)
			{
				if (copy == true)
				cells_data[i][j] = source.cells_data[i][j];
				else
				cells_data[i][j] = cell_free;
			}
		}

	}

	~Mapa()
	{
		
		deallocate();
	
	}

	

	void FloodFill_fill(Point start, Point target,bool diagonal);
	Mapa FloodFill_find_path(Point start, Point target, floodfill_priority priority, std::queue <RobotPosition>& path,bool diagonal,int window_size);

	
	int assert_matrix_indices(Matrix_position XY);
	int check_close_obstacle(Matrix_position XY,int window_size);


	int addPoint(Point P, cell_content content);
	void addPointToHistogram(Point P);
	void buildFromHistogram(Mapa& histogram, int treshold);

	void saveMap(std::string filename);
	void clearMap();
	void loadMap(std::string filename);

private:

	int cols = 0; 
	int rows = 0;
	float  x_lim[2], y_lim[2];
	int** cells_data=NULL;
	
	void allocate()
	{
		cells_data = new int*[rows];
		for (int i = 0; i < rows; ++i)
		{
			cells_data[i] = new int[cols];
			for (int j = 0; j < cols; j++)
				cells_data[i][j] = cell_free;
		}
	}
	void deallocate()
	{
		if (cells_data != NULL)
		{

			for (int i = 0; i < rows; i++) {
				delete[] cells_data[i];
			}
			delete[] cells_data;
			cells_data = NULL;
		}
	}


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
Q_DECLARE_METATYPE(Mapa)

