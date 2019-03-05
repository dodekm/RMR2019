#pragma once
#include "points.h"
#include "rplidar.h"
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <new> 


#define lidar_angle_offset 0.0
#define map_max_size 500

enum
{
	cell_free,
	cell_obstacle
}cell_content;

typedef struct Matrix_position
{
	int X, Y;
}Matrix_position;


Point homogen_transformation(LaserData lidar_measurement, Position robot_position);
float deg2rad(float);
float rad2deg(float);


class Mapa
{

public:
	Mapa()
	{
	
	}
	
	Mapa(int rows,int cols, float  x_low,float x_high,float y_low,float y_high)
	{
		cells = new int*[rows];
		for (int i = 0; i < rows; ++i) 
		{
			cells[i] = new int[cols];
			for (int j = 0; j < cols; j++)
				cells[i][j] = cell_free;
		}
		this->cols = cols;
		this->rows = rows;
		this->x_lim[0] = x_low;
		this->x_lim[1] = x_high;
		this->y_lim[0] = y_low;
		this->y_lim[1] = y_high;

		saveMap("file.txt");
		//loadMap("file.txt");
	}
	
	~Mapa()
	{
		for (int i = 0; i < rows; ++i) {
			delete[] cells[i];
		}
		delete[] cells;
	
	}
	
	int addPoint(Point P);

	void saveMap(std::string filename);

	void loadMap(std::string filename);

private:
	int cols, rows;
	float  x_lim[2], y_lim[2];
	int** cells;
	

	
	Matrix_position point2indices(Point P)
	{
		Matrix_position XY;
		XY.X = (int)round(cols * (P.X - x_lim[0]) / (x_lim[1] - x_lim[0]));
		XY.Y = (int)round(cols * (P.Y - y_lim[0]) / (y_lim[1] - y_lim[0]));
		return XY;
	}
	

};

