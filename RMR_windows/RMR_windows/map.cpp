#include "map.h"
#include "pch.h"


Point lidar_measure_2_point(LaserData lidar_measurement, RobotPosition robot_position)
{
	Point P;
	P.X = robot_position.coordinates.X + lidar_measurement.scanDistance/1000*cos(-deg2rad(lidar_measurement.scanAngle) + robot_position.alfa);
	P.Y = robot_position.coordinates.Y + lidar_measurement.scanDistance/1000*sin(-deg2rad(lidar_measurement.scanAngle) + robot_position.alfa);
	return P;
}

int Mapa::addPoint(Point P)
{
	Matrix_position XY = point2indices(P);
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
	if (!file.is_open())
		return;
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
	file.close();
}

void Mapa::loadMap(std::string filename)
{

	std::fstream file;
	file.open(filename, std::ios::in);
	if (!file.is_open())
		return;

	char str[map_max_size];

	Matrix_position i{0,0};
	int j;

	while (i.Y<rows&&file.getline(str, map_max_size))
	{
		i.X = 0;
		j = 0;
		while (i.X<cols&&str[j]!=';')
		{
			if (str[j] == ',')
				i.X++;
			
			else if(isdigit(str[j]))
			{
				cells[i.Y][i.X] = std::atoi(str + j);
			}
			j++;
		}
		i.Y++;
		

	}
		
	file.close();


}

float deg2rad(float deg)
{
	return deg / 180 * PI;
}

float rad2deg(float rad)
{
	return rad / PI * 180;
}