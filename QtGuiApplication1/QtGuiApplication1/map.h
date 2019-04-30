#pragma once
#define _USE_MATH_DEFINES

#include "points.h"
#include "rplidar.h"
#include "misc.h"
#include "map_loader.h"

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
	cell_free = 0,
	cell_obstacle = 1,
	cell_finish = 2,
	cell_start = -1,
	
	cell_black=1,
	cell_red = 2,
	cell_green=-1,
	cell_blue=-2,
	cell_cyan=-4,
	cell_magenta=-5,
	cell_yellow=-6,

	cell_path = cell_blue,
	cell_breakpoint = cell_yellow,
	cell_robot = cell_magenta,
	cell_direction = cell_yellow,
	cell_slam_estimate = cell_cyan,

	cell_obstacle_edge = cell_green,
	cell_obstacle_edge_in_way = cell_blue


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

	Matrix_position& operator=(const Matrix_position& other)
	{
		X = other.X;
		Y = other.Y;
		return *this;
	}
	

	Matrix_position operator +(const Matrix_position& other)
	{
		return Matrix_position( X + other.X,Y + other.Y );
	}

	Matrix_position operator -(const Matrix_position& other)
	{
		return Matrix_position( X - other.X,Y - other.Y );
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
	
		this->cols =0;
		this->rows =0;
		this->x_lim[0] = 0.0;
		this->x_lim[1] = 0.0;
		this->y_lim[0] = 0.0;
		this->y_lim[1] = 0.0;
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


	Mapa&  operator=(const Mapa& source)
	{
		deallocate();
		cols = source.cols;
		rows = source.rows;
		x_lim[0] = source.x_lim[0];
		x_lim[1] = source.x_lim[1];
		y_lim[0] = source.y_lim[0];
		y_lim[1] = source.y_lim[1];
		allocate();

		for (int i = 0; i < rows; i++)
		{

			for (int j = 0; j < cols; j++)
			{
				cells_data[i][j] = source.cells_data[i][j];
			}
		}
		return *this;
	}

	int& operator[](Matrix_position position)
	{
		if (cells_data != NULL && assert_matrix_indices(position))
		{
			return (cells_data[position.Y][position.X]);
		}	
		
	}

	Mapa operator && (Mapa& other)
	{
		Mapa AND(other, false);
		Matrix_position i;
		for (i.Y = 0; i.Y < rows; i.Y++)
		{
			for (i.X = 0; i.X < cols; i.X++)
			{

				AND[i] = ((*this)[i]&& other[i]);

			}

		}
		
		return AND;
	}


	long sum_elements()
	{
		long count = 0;
		Matrix_position i;
		for (i.Y = 0; i.Y < rows; i.Y++)
		{
			for (i.X = 0; i.X < cols; i.X++)
			{

				count =count+ (*this)[i];

			}

		}
		return count;
	}

	int get_rows() { return rows; }
	int get_cols() { return cols; }

	Mapa (const Mapa& source,bool copy=true)
	{
	
		cols = source.cols;
		rows = source.rows;
		x_lim[0] = source.x_lim[0];
		x_lim[1] = source.x_lim[1];
		y_lim[0] = source.y_lim[0];
		y_lim[1] = source.y_lim[1];

		allocate();

		Matrix_position i;
		if (copy == true)
		{
			for (i.X = 0; i.X < rows; i.X++)
			{

				for (i.Y = 0; i.Y < cols; i.Y++)
				{

					(*this)[i] = source.cells_data[i.Y][i.X];
				}
			}
		}
	}

	~Mapa()
	{
		
		deallocate();
	
	}


	void FloodFill_fill(Point start, Point target,bool diagonal);
	bool FloodFill_find_path(Mapa& map_with_path,Point start, Point target, floodfill_priority priority, std::queue <RobotPosition>& path,bool diagonal);

	std::list<Point> get_obstacles_points();


	int assert_matrix_indices(Matrix_position XY);

	void enhance_obstacles(int window_size);

	int addPoint(Point P, cell_content content);

	void addPointToHistogram(Point P);
	void buildFromHistogram(Mapa& histogram, int treshold);

	void fill_with_objects(map_loader::TMapArea objects);

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
		XY.Y = (int)round(rows * (1.0-((P.Y-y_lim[0]) / (y_lim[1] - y_lim[0]))));
		return XY;
	}


	Point indices2point( Matrix_position XY)
	{
		Point P;
		P.X = (x_lim[1] - x_lim[0])*(float)XY.X / cols + x_lim[0];
		P.Y = (y_lim[1] - y_lim[0])*(1.0-(float)XY.Y / rows) + y_lim[0];
		return P;
	}
	

};
Q_DECLARE_METATYPE(Mapa)

