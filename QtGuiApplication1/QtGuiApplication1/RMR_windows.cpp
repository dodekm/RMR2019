// RMR_windows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "RMR_windows.h"

RobotControll::RobotControll() :

	odometria_1(5.2, 2.5, -M_PI_2),
	odometria_2(5.2, 2.5, -M_PI_2),
	odometria_3(5.2, 2.5, -M_PI_2),
	odometria_4(5.2, 2.5, -M_PI_2),
	regulator(150, 1.2),
	mapa(100, 100, -0.2, 6, -0.2, 6, ""),
	histogram(mapa, false),
	current_scope(100, 100, -lidar_treshold_max / 1000.0, lidar_treshold_max / 1000.0, -lidar_treshold_max / 1000.0, lidar_treshold_max / 1000.0)

{

	command = robot_command::stop;
	odometria_using = &odometria_4;
	actual_position = odometria_using->position;
	reset_position = odometria_using->position;
	slam_position = actual_position;
	odometry_position_last = actual_position;
	odometry_position = actual_position;

	WinSock_setup();

	wanted_position = actual_position;
	start = actual_position.coordinates;
	target = Point{ 0, 0 };

	map_loader::TMapArea objects;
	map_loader::load_objects("priestor.txt", objects);
	mapa.fill_with_objects(objects);

	slam.map_reference = mapa;
	slam.estimate = slam_position;

}

RobotControll::~RobotControll()
{
	stop_threads();
}

void RobotControll::WinSock_setup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		std::cout << "WSAStartup failed " << std::endl;
		return;
	}
}
void RobotControll::robot_controll()
{

	while (threads_enabled == true)
	{
		if (!command_queue.empty())
		{
			command = pop_command();
			switch (command)
			{
			case robot_command::stop:
				mutex_robot_data.lock();
				regulator.enabled = false;
				reset_command_queue();
				motors_speed.translation_speed = 0;
				motors_speed.radius = max_radius;
				mutex_robot_data.unlock();
				break;

			case robot_command::forward:
				mutex_robot_data.lock();
				regulator.enabled = false;
				motors_speed.translation_speed = 200;
				motors_speed.radius = max_radius;
				mutex_robot_data.unlock();
				break;

			case robot_command::back:
				mutex_robot_data.lock();
				regulator.enabled = false;
				motors_speed.translation_speed = -200;
				motors_speed.radius = max_radius;
				mutex_robot_data.unlock();
				break;

			case robot_command::left:
				mutex_robot_data.lock();
				regulator.enabled = false;
				motors_speed.translation_speed = 100;
				motors_speed.radius = d * 1000;
				mutex_robot_data.unlock();
				break;

			case robot_command::right:
				mutex_robot_data.lock();
				regulator.enabled = false;
				motors_speed.translation_speed = 100;
				motors_speed.radius = -d * 1000;
				mutex_robot_data.unlock();
				break;

			case robot_command::automatic:
				mutex_robot_data.lock();
				automode();
				mutex_robot_data.unlock();
				break;

			case robot_command::save:
				mutex_robot_data.lock();
				mapa.saveMap(filename);
				mutex_robot_data.unlock();
				break;

			case robot_command::load:
				mutex_robot_data.lock();
				mapa.loadMap(filename);
				mutex_robot_data.unlock();
				break;
			case robot_command::clear:
				mutex_robot_data.lock();
				mapa.clearMap();
				mutex_robot_data.unlock();
				break;

			case robot_command::reset:
				mutex_robot_data.lock();
				reset_robot();
				mutex_robot_data.unlock();
				break;

			case robot_command::clear_path:
				mutex_robot_data.lock();
				clear_path();
				mutex_robot_data.unlock();
				break;

			case robot_command::find:
				mutex_robot_data.lock();
				find_path(mapa);
				mutex_robot_data.unlock();
				break;
			case robot_command::disconnect:

				break;
			case robot_command::print:

				std::cout << (*this);
				break;

			case robot_command::slam:

				if (motors_speed.translation_speed < min_speed)
				{

					slam_counter++;


					if (slam_counter % slam_modulo_main == 0)
					{
						slam.dispersion_position = 0.2;
						slam.dispersion_angle = 0.2;
						slam.feedback_gain = 0.8;
						slam.odometry_gain = 0.2;
						slam.n_particles = 100;

						slam.locate(odometry_position, Laser_data_working);
						slam_position = slam.estimate;

					}
				}

				else
				{
					slam.estimate_quality = 0;
					slam_counter = 0;
				}

				break;

			case robot_command::build_scope:
				build_scope();
				break;

			case robot_command::build_map:
				build_map();
				break;

			}

		}

	}
	return;
}

void RobotControll::processThisRobot()
{
	mutex_robot_data.lock();

	encoders_process();
	odometria_1.odometry_forward_euler(encL, encR);
	odometria_2.odometry_backward_euler(encL, encR);
	odometria_3.odometry_trapezoidal_rule(encL, encR);
	odometria_4.odometry_curved(encL, encR);

	odometry_position = odometria_using->position;
#ifdef use_slam
	if (slam.estimate_quality > slam.quality_treshold)
	actual_position = slam_position + (odometry_position - odometry_position_last);
	else
	actual_position = actual_position+ (odometry_position - odometry_position_last);
#else
	actual_position = odometry_position;
#endif

	odometry_position_last = odometry_position;

	if (regulator.enabled == true)
	{
		regulator.regulate(actual_position, wanted_position);
		motors_speed = regulator.output;
	}

	move_arc(filter_speed.set_speed((int)round(motors_speed.translation_speed), 50), (int)round(motors_speed.radius));


	emit odometry_update_sig(getRobotData());
	mutex_robot_data.unlock();

}

void RobotControll::clear_path()
{

	while (!path.empty())
	{
		path.pop();
	}

}


void RobotControll::reset_robot()
{
	reset_command_queue();
	clear_path();
	regulator.enabled = false;
	mapa.clearMap();
	histogram.clearMap();
	current_scope.clearMap();
	encoder_init_values(&encL, robotdata.EncoderLeft);
	encoder_init_values(&encR, robotdata.EncoderRight);
	odometria_using->odometry_init(reset_position);
	motors_speed.translation_speed = 0;
	motors_speed.radius = max_radius;
	command = robot_command::stop;

}

void RobotControll::addPointToPath(RobotPosition P)
{
	mutex_robot_data.lock();
	path.push(P);
	mutex_robot_data.unlock();

}



void RobotControll::push_command(robot_command command)
{
	mutex_command_queue.lock();
	command_queue.push(command);
	mutex_command_queue.unlock();
}

robot_command RobotControll::pop_command()
{
	mutex_command_queue.lock();
	robot_command command;
	if (!command_queue.empty())
	{
		command = command_queue.front();
		command_queue.pop();
	}
	mutex_command_queue.unlock();
	return command;

}

void RobotControll::reset_command_queue()
{
	mutex_command_queue.lock();
	while (!command_queue.empty())
	{
		command_queue.pop();
	}
	mutex_command_queue.unlock();

}


void RobotControll::set_maping_enabled(bool status)
{

	maping_enable = status;

}

void RobotControll::set_map_with_path_enabled(bool status)
{

	map_with_path_enable = status;
}


void RobotControll::set_threads_enabled(bool status)
{

	threads_enabled = status;

}

std::string RobotControll::get_command_name()
{
	return command_to_string[command];
}


void RobotControll::set_start(Point start)
{
	mutex_robot_data.lock();
	this->start = start;
	mutex_robot_data.unlock();

}

void RobotControll::set_target(Point target)
{
	mutex_robot_data.lock();
	this->target = target;
	mutex_robot_data.unlock();
}

void RobotControll::setip(std::string ip)
{
	ipaddress = ip;

}

void RobotControll::setfilename(std::string filename)
{

	this->filename = filename;
}


RobotPosition RobotControll::get_position()
{

	return actual_position;
}

RobotPosition RobotControll::get_wanted_position()
{
	return wanted_position;
}


robotSpeed RobotControll::get_motors_speed()
{
	return motors_speed;
}

Point RobotControll::get_target_point()
{
	return target;
}
Point RobotControll::get_starting_point()
{
	return start;
}
std::vector<RobotPosition> RobotControll::get_path()
{

	std::vector<RobotPosition>trajectory;

	std::queue<RobotPosition>path_copy = std::queue<RobotPosition>(path);

	while (!path_copy.empty())
	{
		trajectory.push_back(path_copy.front());
		path_copy.pop();
	}

	return trajectory;
}


Robot_feedback RobotControll::getRobotData()
{

	mutex_command_queue.lock();
	Robot_feedback data{
	 actual_position,
	 wanted_position,
	 wanted_position_corrected,
	 std::list<obstacle>(obstacles),
	 std::list<obstacle>(obstacles_in_way),
	 slam_position,
	 slam.estimate_quality,
	 motors_speed,
	 start,
	 target,
	 command,
	 std::queue<robot_command>(command_queue),
	 get_command_name(),
	 get_path(),
	};
	mutex_command_queue.unlock();
	return data;
}


void RobotControll::printData(std::ostream& stream)
{
	stream << "EncoderDataLeft=" << robotdata.EncoderLeft << "tick" << std::endl;
	stream << "EncoderDataRight=" << robotdata.EncoderRight << "tick" << std::endl;

	stream << "EncoderRealLeft=" << encL.encoder_real_value << "tick" << std::endl;
	stream << "EncoderRealRight=" << encR.encoder_real_value << "tick" << std::endl;

	stream << "Position X_trapezoidal=" << odometria_3.position.coordinates.X << "m" << std::endl;
	stream << "Position Y_trapezoidal=" << odometria_3.position.coordinates.Y << "m" << std::endl;
	stream << "Angle_trapezoidal=" << (odometria_3.position.alfa * 180 / M_PI) << "deg." << std::endl;

	stream << "Position X_curved=" << odometria_4.position.coordinates.X << "m" << std::endl;
	stream << "Position Y_curved=" << odometria_4.position.coordinates.Y << "m" << std::endl;
	stream << "Angle_curved=" << (odometria_4.position.alfa * 180 / M_PI) << "deg." << std::endl;

	stream << "Robot_Mode=" << command << std::endl;

	stream << "Position X_wanted=" << wanted_position.coordinates.X << "m" << std::endl;
	stream << "Position Y_wanted=" << wanted_position.coordinates.Y << "m" << std::endl;

	stream << "vt=" << regulator.getTranslation_output() << "mm/s" << std::endl;
	stream << "R=" << regulator.getRotation_output() << "mm" << std::endl;


}

void RobotControll::obstacle_avoidance()
{
	if (obstacles_in_way.empty())
		return;

	obstacle obst = obstacles_in_way.front();

	if (obst.is_out_of_range(actual_position.coordinates, 0.9*lidar_treshold_max / 1000) == false)
	{
		Point edge_A = obst.points.front();
		Point edge_B = obst.points.back();

		Point shortest_edge;

		float path_A_length = PointsDistance(actual_position.coordinates, edge_A) + PointsDistance(edge_A, wanted_position.coordinates);
		float path_B_length = PointsDistance(actual_position.coordinates, edge_B) + PointsDistance(edge_B, wanted_position.coordinates);

		if (path_A_length < path_B_length)
		{
			wanted_position_corrected.coordinates = edge_A;
		}

		else
		{
			wanted_position_corrected.coordinates = edge_B;
		}


	}


}

bool RobotControll::is_point_in_way(Point m)
{
	//parametricke vyjadrenie priamky 
	Point A = actual_position.coordinates;
	Point B = wanted_position.coordinates;

	float k_x = (B - A).X;
	float k_y = (B - A).Y;

	//vseobecna rovnica priamky
	float a, b, c;

	a = -k_y;
	b = k_x;
	c = -k_x * A.Y + k_y * A.X;


	float dist = abs(a*m.X + b * m.Y + c) / PointLength(Point{ a,b });
	if (dist < zone_width)
		return true;
	return false;

}

bool RobotControll::is_obstacle_in_way(obstacle obst)
{
	if (obst.points.empty())
		return false;
	for (std::list<Point>::iterator it = obst.points.begin(); it != obst.points.end(); it++)
	{
		if (is_point_in_way(*it) == true)
			return true;
	}
	return false;

}

std::list<obstacle> RobotControll::get_obstacles_in_way()
{
	std::list<obstacle> obstacle_list;

	if (obstacles.empty())
		return obstacle_list;

	for (std::list<obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); it++)
	{
		if (is_obstacle_in_way(*it) == true)
			obstacle_list.push_back(*it);
	}
	return obstacle_list;

}

std::list<obstacle> RobotControll::find_obstacles(std::list<Point>points)
{
	std::list<obstacle> obstacles;

	if (points.empty())
		return std::list<obstacle>();

	std::list<Point>candidates(points);

	while (!candidates.empty())
	{
		obstacle obst;
		Point working_point;
		std::list<Point>::iterator it;
		std::list<Point>::iterator starting_point = candidates.begin();

		obst.points.push_back(*starting_point);
		working_point = *candidates.begin();
		while (1)
		{
			std::list<Point>::iterator closest_point = candidates.begin();
			float min_distance;
			for (it = candidates.begin(); it != candidates.end(); it++)
			{
				if (it == candidates.begin() ||(PointsDistance(working_point, *it) < min_distance && it != starting_point))
				{
					min_distance = PointsDistance(working_point, *it);
					closest_point = it;
				}
			}
			if (min_distance < point_dist_treshold)
			{
				obst.points.push_back(*closest_point);
				working_point = *closest_point;
				candidates.erase(closest_point);
			}
			else
			{
				break;
			}

		}

		working_point = *starting_point;
		while (1)
		{
			std::list<Point>::iterator closest_point = candidates.begin();
			float min_distance;
			
			if (candidates.empty())
				break;

			for (it = candidates.begin(); it != candidates.end(); it++)
			{
				if (it == candidates.begin() || (PointsDistance(working_point, *it) < min_distance && it != starting_point))
				{
					min_distance = PointsDistance(working_point, *it);
					closest_point = it;
				}
			}
			if (min_distance < zone_width)
			{
				obst.points.push_front(*closest_point);
				working_point = *closest_point;
				candidates.erase(closest_point);
			}
			else
			{
				break;
			}

		}
		candidates.erase(starting_point);
		obstacles.push_back(obst);


	}
	return obstacles;

}

void RobotControll::automode()
{
	
	if (!obstacles_in_way.empty())
	{
		
		Mapa merged_map(mapa);
		if (!current_scope_obstacles.empty())
		{
			for (std::list<Point>::iterator it = current_scope_obstacles.begin(); it != current_scope_obstacles.end(); it++)
			{
				merged_map.addPoint(*it, cell_obstacle);
			}
			find_path(merged_map);
			push_command(robot_command::stop);
			regulator.enabled = false;
			return;
		}

	}
	
		
		if (regulator.isRegulated(actual_position, wanted_position))
		{
			if (!path.empty())
			{
		#ifdef use_slam
				if (slam.estimate_quality > slam.quality_treshold)
		#endif 
				{
					wanted_position = path.front();
					path.pop();
				}
				push_command(robot_command::automatic);
			
			}
			else
			{
				push_command(robot_command::stop);
				regulator.enabled = false;
			}
		}
		else
		{
			push_command(robot_command::automatic);
			regulator.enabled = true;
		}
	

	
}

void RobotControll::build_scope()
{

	if (Laser_data_new.size() > lidar_scan_modulo)
	{
		mutex_lidar_data.lock();
		Laser_data_working = Laser_data_new;
		Laser_data_new.clear();
		mutex_lidar_data.unlock();


		current_scope.clearMap();
		current_scope_obstacles.clear();

		current_scope.addPoint(Point{ 0.0,0.0 }, cell_robot);
		for (int i = 0; i < Laser_data_working.size(); i++)
		{
			if (lidar_check_measure(Laser_data_working[i]))
			{
				current_scope.addPoint(lidar_measure_2_point(Laser_data_working[i], RobotPosition(0, 0, M_PI_2)), cell_obstacle);
				current_scope_obstacles.push_back(lidar_measure_2_point(Laser_data_working[i], actual_position));

			}
		}
		mutex_robot_data.lock();
		obstacles=find_obstacles(current_scope_obstacles);
		obstacles_in_way = get_obstacles_in_way();
		mutex_robot_data.unlock();

		emit scope_update_sig(current_scope);
	}

	

}

void RobotControll::build_map()
{

	if (motors_speed.translation_speed == 0 && maping_enable == true)
	{

		for (int i = 0; i < Laser_data_working.size(); i++)
		{

			if (lidar_check_measure(Laser_data_working[i]))
			{
				histogram.addPointToHistogram(lidar_measure_2_point(Laser_data_working[i], actual_position));
				lidar_measure_counter++;
			}


			if (lidar_measure_counter % lidar_build_modulo == 0)
			{
				lidar_measure_counter = 0;
				mapa.buildFromHistogram(histogram, histogram_treshold);
				histogram.clearMap();
			}
		}
	}
	Mapa map_to_send(mapa);
	map_to_send.addPoint(actual_position.coordinates, cell_slam_estimate);
	map_to_send.addPoint(odometry_position.coordinates, cell_robot);
	map_to_send.addPoint(actual_position.coordinates + polar2point(actual_position.alfa, 0.2), cell_direction);
	map_to_send.addPoint(target, cell_finish);

	
	if (!obstacles.empty())
	{
		for (std::list<obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); it++)
		{
			Point P_1 = (*it).points.front();
			Point P_2 = (*it).points.back();
			map_to_send.addPoint(P_1, cell_obstacle_corner);
			map_to_send.addPoint(P_2, cell_obstacle_corner);
		}
	}
	
	if (!obstacles_in_way.empty())
	{
		for (std::list<obstacle>::iterator it = obstacles_in_way.begin(); it != obstacles_in_way.end(); it++)
		{
			Point P_1 = (*it).points.front();
			Point P_2 = (*it).points.back();
			map_to_send.addPoint(P_1, cell_obstacle_corner_in_way);
			map_to_send.addPoint(P_2, cell_obstacle_corner_in_way);
		}
	}
	
	
	if (map_with_path_enable == false)
	{
		emit map_update_sig(map_to_send);
	}
	else
	{
		emit(map_update_sig(map_with_path));
	}
}

void RobotControll::find_path(Mapa working_map)
{
	start = actual_position.coordinates;
	int window_size = 5;
	Mapa mapa_flood_fill(working_map, true);
	mapa_flood_fill.FloodFill_fill(start, target, true);
	clear_path();
	map_with_path = mapa_flood_fill.FloodFill_find_path(start, target, floodfill_priority_Y, path, true, window_size);
	if (!path.empty())
		wanted_position = path.front();
	

}




void RobotControll::encoders_process()
{
	encoder_process(&encL, robotdata.EncoderLeft);
	encoder_process(&encR, robotdata.EncoderRight);
}


void RobotControll::processThisLidar(std::vector<LaserData> new_scan)
{
	mutex_lidar_data.lock();
	if (!new_scan.empty())
	{
		for (std::vector<LaserData>::iterator i = new_scan.begin(); i != new_scan.end(); i++)
		{
			Laser_data_new.push_back(*i);
		}
	}
	mutex_lidar_data.unlock();

	push_command(robot_command::build_scope);
	push_command(robot_command::build_map);
#ifdef use_slam
	push_command(robot_command::slam);
#endif
}


void RobotControll::start_threads()
{
	
	threads_enabled = true;
	if (!robotthreadHandle.joinable())
		robotthreadHandle = std::thread(&RobotControll::robotprocess, this);
	if (!laserthreadHandle.joinable())
		laserthreadHandle = std::thread(&RobotControll::laserprocess, this);
	if (!controllhreadHandle.joinable())
		controllhreadHandle = std::thread(&RobotControll::robot_controll, this);

}

void  RobotControll::stop_threads()
{
	threads_enabled = false;
	if (robotthreadHandle.joinable())
		robotthreadHandle.detach();
	if (laserthreadHandle.joinable())
		laserthreadHandle.detach();
	if (controllhreadHandle.joinable())
		controllhreadHandle.detach();

}


void RobotControll::move_arc(int mmpersec, int radius) //forward
{

	std::vector<unsigned char> mess = robot.setArcSpeed(mmpersec, radius);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}

void RobotControll::forward(int mmpersec) //forward
{
	//pohyb dopredu
	std::vector<unsigned char> mess = robot.setTranslationSpeed(mmpersec);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}

void RobotControll::back(int mmpersec) //back
{
	std::vector<unsigned char> mess = robot.setTranslationSpeed(-mmpersec);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}

void RobotControll::left(double radpersec) //left
{

	std::vector<unsigned char> mess = robot.setRotationSpeed(radpersec);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}

void RobotControll::right(double radpersec)//right
{

	std::vector<unsigned char> mess = robot.setRotationSpeed(-radpersec);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}

void RobotControll::stop() //stop
{
	std::vector<unsigned char> mess = robot.setTranslationSpeed(0);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}


///tato funkcia vas nemusi zaujimat
/// toto je funkcia s nekonecnou sluckou,ktora cita data z lidaru (UDP komunikacia)
void RobotControll::laserprocess()
{


	las_slen = sizeof(las_si_other);
	if ((las_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		std::cout << "Socket not created" << std::endl;
		return;
	}
	int las_broadcastene = 1;
	setsockopt(las_s, SOL_SOCKET, SO_BROADCAST, (char*)&las_broadcastene, sizeof(las_broadcastene));

	memset((char *)&las_si_me, 0, sizeof(las_si_me));

	las_si_me.sin_family = AF_INET;
	las_si_me.sin_port = htons(52999);//toto je port z ktoreho pocuvame
	las_si_me.sin_addr.s_addr = htonl(INADDR_ANY);//moze dojst od hocikial..

	las_si_posli.sin_family = AF_INET;
	las_si_posli.sin_port = htons(5299);//toto je port na ktory posielame
	las_si_posli.sin_addr.s_addr = inet_addr(ipaddress.data());//htonl(INADDR_BROADCAST);

	int result = bind(las_s, (struct sockaddr*)&las_si_me, sizeof(las_si_me));
	if (result == SOCKET_ERROR)
	{
		std::cout << "laser connect error" << std::endl;
		return;
	}
	char command = 0x00;
	if (sendto(las_s, &command, sizeof(command), 0, (struct sockaddr*) &las_si_posli, las_slen) == -1)
	{
		std::cout << "send error" << std::endl;
		return;
	}


	LaserMeasurement measure;
	while (threads_enabled == true)
	{

		if ((las_recv_len = recvfrom(las_s, (char*)&measure.Data, sizeof(LaserData) * 1000, 0, (struct sockaddr *) &las_si_other, &las_slen)) == -1)
		{

			continue;
		}
		measure.numberOfScans = las_recv_len / sizeof(LaserData);


		processThisLidar(std::vector<LaserData>(measure.Data, measure.Data + measure.numberOfScans));

	}

	closesocket(las_s);

	return;

}

///tato funkcia vas nemusi zaujimat
/// toto je funkcia s nekonecnou sluckou,ktora cita data z robota (UDP komunikacia)
void RobotControll::robotprocess()
{


	if ((rob_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		std::cout << "Socket not created" << std::endl;
		return;
	}

	char rob_broadcastene = 1;
	setsockopt(rob_s, SOL_SOCKET, SO_BROADCAST, &rob_broadcastene, sizeof(rob_broadcastene));
	// zero out the structure
	memset((char *)&rob_si_me, 0, sizeof(rob_si_me));

	rob_si_me.sin_family = AF_INET;
	rob_si_me.sin_port = htons(53000);
	rob_si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	rob_si_posli.sin_family = AF_INET;
	rob_si_posli.sin_port = htons(5300);
	rob_si_posli.sin_addr.s_addr = inet_addr(ipaddress.data());
	rob_slen = sizeof(rob_si_me);

	int result = bind(rob_s, (struct sockaddr*)&rob_si_me, sizeof(rob_si_me));
	if (result == SOCKET_ERROR)
	{
		std::cout << "robot connect error" << std::endl;
		return;
	}
	std::vector<unsigned char> mess = robot.setDefaultPID();
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{
		std::cout << "send error" << std::endl;
		return;
	}

	Sleep(100);
	mess = robot.setSound(440, 1000);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{
		std::cout << "send error" << std::endl;
		return;
	}

	unsigned char buff[50000];


	while (threads_enabled == true)
	{


		memset(buff, 0, 50000 * sizeof(char));
		if ((rob_recv_len = recvfrom(rob_s, (char*)&buff, sizeof(char) * 50000, 0, (struct sockaddr *) &rob_si_other, &rob_slen)) == -1)
		{

			continue;
		}


		int returnval = robot.fillData(robotdata, (unsigned char*)buff);
		if (returnval == 0)
		{
			if (datacounter == 0)
			{
				encoder_init_values(&encL, robotdata.EncoderLeft);
				encoder_init_values(&encR, robotdata.EncoderRight);
			}
			datacounter++;
			processThisRobot();
		}
	}
	closesocket(rob_s);

	return;
}

