#include "map.h"
#include "pch.h"


Point homogen_transformation(LaserData lidar_measurement, Position robot_position)
{
	Point P;
	P.X = robot_position.coordinates.X + lidar_measurement.scanDistance/1000*cos(2*PI-deg2rad(lidar_measurement.scanAngle) + robot_position.alfa);
	P.Y = robot_position.coordinates.Y + lidar_measurement.scanDistance/1000*sin(2*PI-deg2rad(lidar_measurement.scanAngle) + robot_position.alfa);

	return P;
}

int Mapa::addPoint(Point P)
{
	Matrix_position XY = Mapa::point2indices(P);
	if (XY.X >= 0 && XY.X < cols&&XY.Y >= 0 && XY.Y < rows)
	{
		if (cells[XY.X][XY.Y] == cell_obstacle)
			return 0;
		cells[XY.X][XY.Y] = cell_obstacle;
		return 1;
	}
	return -1;


}

void Mapa::saveMap(std::string filename)
{
	std::fstream file;
	file.open(filename,std::ios::out|std::ios::trunc);
	
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			file << cells[i][j];
			if (j  < rows-1)
				file << ',';
			else
			{
				file << ';';
				file << '\n';
			}
		}
		
	}

	file.flush();
}

void Mapa::loadMap(std::string filename)
{



}

float deg2rad(float deg)
{
	return deg / 180 * PI;
}

float rad2deg(float rad)
{
	return rad / PI * 180;
}