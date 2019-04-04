

#include "map.h"


using namespace std;

Point lidar_measure_2_point(LaserData lidar_measurement, RobotPosition robot_position)
{
	Point P;
	P.X = robot_position.coordinates.X + lidar_measurement.scanDistance / 1000 * cos(-deg2rad(lidar_measurement.scanAngle) + robot_position.alfa);
	P.Y = robot_position.coordinates.Y + lidar_measurement.scanDistance / 1000 * sin(-deg2rad(lidar_measurement.scanAngle) + robot_position.alfa);
	return P;
}


int Mapa::assert_matrix_indices(Matrix_position XY)
{
	return(XY.X >= 0 && XY.X < cols&&XY.Y >= 0 && XY.Y < rows);
}


int Mapa::addPoint(Point P,cell_content content=cell_obstacle)
{
	Matrix_position XY = point2indices(P);
	if (assert_matrix_indices(XY))
	{
		if ((*this)[XY] == cell_obstacle)
			return 0;
		(*this)[XY] = content;
		return 1;
	}
	return -1;

}

void Mapa::addPointToHistogram(Point P)
{
	Matrix_position XY = point2indices(P);
	if (assert_matrix_indices(XY))
	{
		(*this)[XY]++;
	}

}

void Mapa::buildFromHistogram(Mapa& histogram, int treshold)
{
	Matrix_position i;
	for (i.Y = 0; i.Y < rows; i.Y++)
	{

		for (i.X = 0; i.X < cols; i.X++)
		{

			if (histogram[i] > treshold)
			{
				(*this)[i] = cell_obstacle;
			}

		}
	}

}

void Mapa::saveMap(std::string filename)
{
	fstream file;
	file.open(filename, ios::out | ios::trunc);
	if (!file.is_open())
		return;

	Matrix_position i;

	for (i.Y = 0; i.Y < rows; i.Y++)
	{
		for (i.X = 0; i.X < cols; i.X++)
		{
			file << (*this)[i];
			if (i.X < rows - 1)
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

	fstream file;
	file.open(filename, ios::in);
	if (!file.is_open())
		return;

	Matrix_position i{ 0,0 };
	std::string str;

	while (i.Y < rows&&getline(file, str))
	{
		vector<string>data= string_split_delim(str, ",");
		i.X = 0;
		while (i.X < cols&&i.X < data.size())
		{
			(*this)[i] = atoi(data.at(i.X).c_str());
			i.X++;
		}

		i.Y++;
	}

	file.close();

}

void Mapa::fill_with_objects(map_loader::TMapArea objects)
{
	
	objects.obstacle.push_back(objects.wall);

	for (int i=0;i<objects.obstacle.size();i++)
	{
		for (int k = 0; k < objects.obstacle.at(i).points.size()-1; k++)
		{

			Point begin = objects.obstacle.at(i).points.at(k);
			Point end = objects.obstacle.at(i).points.at(k+1);
			
			end.X /= 100;
			end.Y /= 100;
			
			begin.X /= 100;
			begin.Y /= 100;

			Matrix_position begin_ind = point2indices(begin);
			Matrix_position end_ind = point2indices(end);
			Matrix_position iter = begin_ind;
			
			while (iter != end_ind)
			{
				Matrix_position direction = Matrix_position(sign(end_ind.X - iter.X), sign(end_ind.Y - iter.Y));
				iter = iter + direction;
				if (assert_matrix_indices(iter))
				{
					(*this)[iter] = cell_obstacle;
				}
			}

		}
		
	
	}


}

void Mapa::clearMap()
{
	Matrix_position i;

	for (i.Y = 0; i.Y < rows; i.Y++)
	{

		for (i.X = 0; i.X < cols; i.X++)
		{
			(*this)[i] = cell_free;
		}
	}

}


void Mapa::FloodFill_fill(Point start, Point target, bool diagonal = false)
{
	Matrix_position target_indices = point2indices(target);
	Matrix_position start_indices = point2indices(start);

	if (!(assert_matrix_indices(target_indices) && assert_matrix_indices(start_indices)))
		return;
	if ((*this)[target_indices] == cell_obstacle || (*this)[start_indices] == cell_obstacle)
		return;

	(*this)[target_indices] = cell_finish;
	(*this)[start_indices] = cell_start;

	Matrix_position working_cell = target_indices;

	std::queue<Matrix_position> points_to_evaluate;

	std::vector<Matrix_position> offset;

	if (diagonal == true)
	{

		offset.push_back(Matrix_position{ 1, 1 });
		offset.push_back(Matrix_position{ -1, -1 });
		offset.push_back(Matrix_position{ 1, -1 });
		offset.push_back(Matrix_position{ -1, 1 });
	}

	offset.push_back(Matrix_position{ -1,0 });
	offset.push_back(Matrix_position{ 1,0 });
	offset.push_back(Matrix_position{ 0,-1 });
	offset.push_back(Matrix_position{ 0, 1 });


	unsigned long iter = 0;
	unsigned long max_iter = cols * rows;
	while (working_cell != start_indices)
	{
		if (iter > max_iter)
			break;
		iter++;

		if (assert_matrix_indices(working_cell))
		{

			for (int i = 0; i < offset.size(); i++)

			{
				Matrix_position new_position = working_cell + offset[i];

				if (assert_matrix_indices(new_position))
				{
					if (new_position == start_indices)
					{
						points_to_evaluate.push(new_position);
					}
					else if ((*this)[new_position] == cell_free)
					{
						(*this)[new_position] = (*this)[working_cell] + 1;
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


Mapa Mapa::FloodFill_find_path(Point start, Point target, floodfill_priority priority, std::queue <RobotPosition>& path, bool diagonal = false, int window_size = 3)
{

	Mapa map_with_path = Mapa(*this, true);
	std::vector<Matrix_position> trajectory;

	Matrix_position target_indices = point2indices(target);
	Matrix_position start_indices = point2indices(start);

	if (!(assert_matrix_indices(target_indices) && assert_matrix_indices(start_indices)))
		return Mapa();
	if (cells_data[target_indices.Y][target_indices.X] != cell_finish)
		return Mapa();
	if (cells_data[start_indices.Y][start_indices.X] != cell_start)
		return Mapa();

	Matrix_position working_cell = start_indices;

	std::vector<Matrix_position> offset;

	if (diagonal&&priority != floodfill_priority_diag)
	{

		offset.push_back(Matrix_position{ -1,-1 });
		offset.push_back(Matrix_position{ 1,1 });
		offset.push_back(Matrix_position{ -1,1 });
		offset.push_back(Matrix_position{ 1,-1 });

	}

	if (priority == floodfill_priority_X)
	{
		offset.push_back(Matrix_position{ 0,-1 });
		offset.push_back(Matrix_position{ 0, 1 });
		offset.push_back(Matrix_position{ -1,0 });
		offset.push_back(Matrix_position{ 1,0 });

	}

	else if (priority == floodfill_priority_Y || priority == floodfill_priority_diag)
	{
		offset.push_back(Matrix_position{ -1,0 });
		offset.push_back(Matrix_position{ 1,0 });
		offset.push_back(Matrix_position{ 0,-1 });
		offset.push_back(Matrix_position{ 0, 1 });
	}

	if (diagonal&&priority == floodfill_priority_diag)
	{

		offset.push_back(Matrix_position{ -1,-1 });
		offset.push_back(Matrix_position{ 1,1 });
		offset.push_back(Matrix_position{ -1,1 });
		offset.push_back(Matrix_position{ 1,-1 });



	}

	unsigned long iter = 0;
	unsigned long max_iter = cols * rows;
	Matrix_position working_cell_prev = Matrix_position(-1, -1);
	while (working_cell != target_indices)
	{
		if (iter > max_iter)
			break;
		iter++;

		int minvalue = INT32_MAX;
		Matrix_position direction = { 0,0 };
		Matrix_position gradient = { 0,0 };
		for (int i = 0; i < offset.size(); i++)
		{
			Matrix_position new_position = working_cell + offset[i];

			if (assert_matrix_indices(new_position))
			{

				if ((*this)[new_position] > cell_obstacle)
				{
					if (this->check_close_obstacle(new_position, window_size))
					{
						
							if ((*this)[new_position] <= minvalue)
							{
								if ((*this)[new_position] == minvalue)
								{
									if (gradient == (new_position - working_cell) || gradient == Matrix_position{ 0,0 })
									{
										direction = new_position;
										(*this)[new_position] = cell_path;
									}
									
								}
								else
								{
									direction = new_position;
									minvalue = (*this)[new_position];
									(*this)[new_position] = cell_path;
								
								}

								
							}
							
								
						
					}

				}

			}
		}
		working_cell_prev = working_cell;
		working_cell = direction;
		gradient = working_cell - working_cell_prev;
		trajectory.push_back(working_cell);
	}
	

	Matrix_position difference_backward = { 0,0 };
	Matrix_position difference_forward = { 0,0 };

	for (int i = 1; i < trajectory.size() - 1; i++)
	{
		Matrix_position working_cell = trajectory[i];
		difference_backward = working_cell - trajectory[i - 1];
		difference_forward = trajectory[i + 1] - working_cell;
		Matrix_position second_order_difference = difference_forward - difference_backward;

		map_with_path[working_cell] = cell_path;

		if (second_order_difference != Matrix_position{ 0,0 })
		{
			path.push(RobotPosition(indices2point(working_cell)));
			map_with_path[working_cell] = cell_breakpoint;

		}
	}

	path.push(RobotPosition(indices2point(target_indices)));
	return map_with_path;

}

int Mapa::check_close_obstacle(Matrix_position XY, int window_size = 2)
{

	Matrix_position offset;
	for (offset.Y = -window_size ; offset.Y <= window_size; offset.Y++)
	{
		for (offset.X = -window_size ; offset.X <= window_size; offset.X++)
		{

			if (assert_matrix_indices(XY + offset))
			{
				if ((*this)[XY + offset] == cell_obstacle)
					return 0;
			}
		}
	}
	return 1;
}

float deg2rad(float deg)
{
	return deg / 180 * M_PI;
}

float rad2deg(float rad)
{
	return rad / M_PI * 180;

}


