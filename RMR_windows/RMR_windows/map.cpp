#include "map.h"
#include "pch.h"


Point lidar_measure_2_point(LaserData lidar_measurement, RobotPosition robot_position)
{
	Point P;
	P.X = robot_position.coordinates.X + lidar_measurement.scanDistance/1000*cos(-deg2rad(lidar_measurement.scanAngle) + robot_position.alfa);
	P.Y = robot_position.coordinates.Y + lidar_measurement.scanDistance/1000*sin(-deg2rad(lidar_measurement.scanAngle) + robot_position.alfa);
	return P;
}

int Mapa::assert_matrix_indices(Matrix_position XY)
{
return(XY.X >= 0 && XY.X < cols&&XY.Y >= 0 && XY.Y < rows);
}


int Mapa::addObstacle(Point P)
{
	Matrix_position XY = point2indices(P);
	if (assert_matrix_indices(XY))
	{
		if (cells_data[XY.X][XY.Y] == cell_obstacle)
			return 0;
		cells_data[XY.X][XY.Y] = cell_obstacle;
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
	
	//file << '[';
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			file << cells_data[i][j];
			if (j  < rows-1)
				file << ',';
			else
			{
				file << ';';
				file << '\n';
			}
		}
		
	}
	//file << ']';
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
				cells_data[i.Y][i.X] = std::atoi(str + j);
			}
			j++;
		}
		i.Y++;
		

	}
		
	file.close();


}
void Mapa::FloodFill_fill(Point start, Point target)
{
	Matrix_position target_indices = point2indices(target);
	Matrix_position start_indices = point2indices(start);

	if (!(assert_matrix_indices(target_indices) && assert_matrix_indices(start_indices)))
		return;

	cells_data[target_indices.Y][target_indices.X] = cell_finish;
	cells_data[start_indices.Y][start_indices.X] = cell_start;

	Matrix_position working_cell = target_indices;

	std::queue<Matrix_position> points_to_evaluate;

	
	while (!(working_cell==start_indices))
	{

		if (assert_matrix_indices(working_cell))
		{

			std::vector<Matrix_position> offset;
			offset.push_back(Matrix_position{ -1,0 });
			offset.push_back(Matrix_position{  1,0 });
			offset.push_back(Matrix_position{ 0,-1 });
			offset.push_back(Matrix_position{ 0, 1 });

			for (int i = 0; i < offset.size();i++)

			{
				Matrix_position new_position = working_cell+offset[i];
				
				
				if (assert_matrix_indices(new_position))
				{
					if (new_position==start_indices)
					{
						points_to_evaluate.push(new_position);
						break;
					}
					
					else if (cells_data[new_position.Y][new_position.X] == cell_free)
					{
						cells_data[new_position.Y][new_position.X] = cells_data[working_cell.Y][working_cell.X] + 1;
						points_to_evaluate.push(new_position);
						
					}
				

				}
			}

			if (points_to_evaluate.empty())
				break;
			working_cell = points_to_evaluate.front();
			points_to_evaluate.pop();

		}
	}

}


std::queue <RobotPosition>  Mapa::FloodFill_find_path(Point start, Point target,int priority)
{
	std::queue <RobotPosition> path;

	Matrix_position target_indices = point2indices(target);
	Matrix_position start_indices = point2indices(start);

	if (!(assert_matrix_indices(target_indices) && assert_matrix_indices(start_indices)))
		return std::queue<RobotPosition>();
	if(cells_data[target_indices.Y][target_indices.X] != cell_finish)
		return std::queue<RobotPosition>();
	cells_data[start_indices.Y][start_indices.X] != cell_start;
		return std::queue<RobotPosition>();

		Matrix_position working_cell = start_indices;

		while (!(working_cell == target_indices))
		{


		}



	return path;
}

float deg2rad(float deg)
{
	return deg / 180 * PI;
}

float rad2deg(float rad)
{
	return rad / PI * 180;
}