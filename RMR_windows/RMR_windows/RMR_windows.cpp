// RMR_windows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "RMR_windows.h"

int main()
{
	RobotControll robot1;
	robot1.start_threads();
	std::string command;
	

	while (1)
	{
		
		std::getline(std::cin, command);
		
	 if (command == "go")
	{
		float x, y;
		std::cin >> x;
		std::cin >> y;
		robot1.addPointToPath(RobotPosition(x, y));
		
		robot1.set_target(Point{ x,y });
		robot1.set_start(robot1.get_position().coordinates);
	 }
	 else  if (command != "")
	 {
		 robot1.set_command(command);
	 }
	}

	robot1.robotthreadHandle.std::thread::join();
	robot1.laserthreadHandle.std::thread::join();
	robot1.guithreadHandle.std::thread::join();
}




RobotControll::RobotControll() :

	regulator(200, 0.5),
	mapa(100, 100, -5.0, 5, -5.0, 5.0,"file.txt"),
	histogram(mapa,false)
{
	command = "stop";
	command_old = "stop";
	
	odometria_using = &odometria_3;

	WinSock_setup();
	std::cout << "Zadaj IP adresu:" << std::endl;
	
		std::string str;
		std::cin >> str;

		if (str.length()==12)
		{
			ipaddress = str;
		}

	path.push(RobotPosition(0.0, 0.0));
	
	start = Point{0, 0 };
	target = Point{ 3, -3 };
	

	find_path();
	
}


RobotControll::~RobotControll()
{

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


void RobotControll::processThisRobot()
{
	if (datacounter == 0)
	{
		encoder_init_values(&encL, robotdata.EncoderLeft);
		encoder_init_values(&encR, robotdata.EncoderRight);
	}


	if (datacounter % modulo_print == 0)
	{
		system("cls");
		std::cout << (*this);
	
	}

	if (datacounter % modulo_odometry == 0)
	{
		
		encoders_process();
		odometria_1.odometry_forward_euler(encL ,encR);
		odometria_2.odometry_backward_euler(encL, encR);
		odometria_3.odometry_trapezoidal_rule(encL, encR);
		odometria_4.odometry_curved(encL, encR);
		actual_position = odometria_using->position;
	
	}

	if (datacounter % modulo_drive == 0)
	{
		if (command == "stop")
		{
			motors_speed.translation_speed=0;
			motors_speed.radius = max_radius;
			
		}
		else if (command == "forward")
		{
			motors_speed.translation_speed=200;
			motors_speed.radius = max_radius;
		}
		else if (command == "back")
		{
			motors_speed.translation_speed=-200;
			motors_speed.radius = max_radius;
		}
		else if (command == "left")
		{
			motors_speed.translation_speed=100;
			motors_speed.radius = d*1000;
			
		}
		else if (command == "right")
		{
			
			motors_speed.translation_speed=100;
			motors_speed.radius = -d*1000;
		}
		

		else if (command == "auto")
		{
			automode();
		}
		
		
		else if (command == "save")
		{
		
			mapa.saveMap("file.txt");
			command_reset();
		}

		else if (command == "load")
		{
			mapa.loadMap("file.txt");
			command_reset();
		}

		else if (command == "find")
		{
			find_path();
			command_reset();
		}


		else if (command == "reset")
		{
			reset_robot();
			
		}

		else if (command == "clear")
		{
			
			mapa.clearMap();
			command_reset();

		}
		else
		{
			command_reset();
		}

		move_arc(filter.set_speed((int)round(motors_speed.translation_speed),speed_filter_steps), (int)round(motors_speed.radius));

	}

	datacounter++;
}

void RobotControll::reset_robot()
{
	while (!path.empty())
	{
		path.pop();
	}
	mapa.clearMap();
	histogram.clearMap();
	encoder_init_values(&encL, robotdata.EncoderLeft);
	encoder_init_values(&encR, robotdata.EncoderRight);
	odometria_using->odometry_init();
	motors_speed.translation_speed = 0;
	motors_speed.radius = max_radius;
	command == "stop";
}

void RobotControll::addPointToPath(RobotPosition P)
{
	path.push(P);
}

void RobotControll::set_command(std::string command)
{
	this->command_old = this->command;
	this->command = command;
}
std::string RobotControll::get_command()
{
	return command;
}

void RobotControll::command_reset()
{
	command = command_old;
}


void RobotControll::set_start(Point start)
{
	this->start = start;
	
}

void RobotControll::set_target(Point target)
{
	this->target = target;
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
	
	std::queue<RobotPosition>path_copy = path;

	while (!path_copy.empty())
	{
		trajectory.push_back(path_copy.front());
		path_copy.pop();
	}
	return trajectory;
}



Mapa RobotControll::getMap()
{
	return mapa;
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
	stream << "lidar_counter" << lidar_measure_counter << std::endl;

}


void RobotControll::automode()
{
	if (!regulator.isRegulated(actual_position, wanted_position))
	{
		regulator.regulate(actual_position, wanted_position);
		motors_speed = regulator.output;
	}
	else 
	{
		motors_speed.translation_speed = 0;
		if (!path.empty())
		{
			wanted_position = path.front();
			path.pop();
		}
	}
}

void RobotControll::build_map()
{
	
	for(int i = 0; i<copyOfLaserData.numberOfScans; i++)
	{
		if (lidar_check_measure(copyOfLaserData.Data[i]))
		{
			histogram.addPointToHistogram(lidar_measure_2_point(copyOfLaserData.Data[i], actual_position));
			lidar_measure_counter++;
		}
		if (lidar_measure_counter % lidar_measure_modulo== 0)
		{
			lidar_measure_counter = 0;
			mapa.buildFromHistogram(histogram, histogram_treshold);
			histogram.clearMap();
		}
	}
	
}

void RobotControll::find_path()
{
	int window_size=2;
	Mapa mapa_flood_fill(mapa,true);
	mapa_flood_fill.FloodFill_fill(start, target, true);
	mapa_flood_fill.saveMap("floodfill.txt");
	Mapa map_with_path = mapa_flood_fill.FloodFill_find_path(start, target, floodfill_priority_Y, path, true, window_size);
	map_with_path.saveMap("path.txt");
	

}


void RobotControll::encoders_process()
{
	encoder_process(&encL, robotdata.EncoderLeft);
	encoder_process(&encR, robotdata.EncoderRight);

}


void RobotControll::processThisLidar(LaserMeasurement &laserData)
{
	
	
	if(command=="stop"&&motors_speed.translation_speed < min_speed&&motors_speed.radius>max_radius/10)
	{
		memcpy(&copyOfLaserData, &laserData, sizeof(LaserMeasurement));
		build_map();
	}
	else
	{
		lidar_measure_counter = 1;
	}
}


void RobotControll::start_threads()
{


	robotthreadHandle = std::thread(robotUDPVlakno, (void *)this);
	std::cout << "Thread1 Started" << std::endl;
	laserthreadHandle = std::thread(laserUDPVlakno, (void *)this);
	std::cout << "Thread2 Started" << std::endl;
	guithreadHandle = std::thread(GUI_Vlakno, (void *)this);
	std::cout << "Thread2 Started" << std::endl;

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
	std::cout << "running laserprocess" << std::endl;


	las_slen = sizeof(las_si_other);
	if ((las_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		std::cout << "Socket not created" << std::endl;
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
		std::cout << "laser connect error" << std::endl;

	char command = 0x00;
	if (sendto(las_s, &command, sizeof(command), 0, (struct sockaddr*) &las_si_posli, las_slen) == -1)
	{
		std::cout << "send error" << std::endl;
	}

	std::cout << "Entering lidar loop" << std::endl;
	LaserMeasurement measure;
	while (1)
	{

		if ((las_recv_len = recvfrom(las_s, (char*)&measure.Data, sizeof(LaserData) * 1000, 0, (struct sockaddr *) &las_si_other, &las_slen)) == -1)
		{

			continue;
		}
		measure.numberOfScans = las_recv_len / sizeof(LaserData);
		processThisLidar(measure);


	}


}

///tato funkcia vas nemusi zaujimat
/// toto je funkcia s nekonecnou sluckou,ktora cita data z robota (UDP komunikacia)
void RobotControll::robotprocess()
{

	std::cout << "running robotprocess" << std::endl;
	if ((rob_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		std::cout << "Socket not created" << std::endl;
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
	}
	std::vector<unsigned char> mess = robot.setDefaultPID();
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{
		std::cout << "send error" << std::endl;

	}

	Sleep(100);
	mess = robot.setSound(440, 1000);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{
		std::cout << "send error" << std::endl;
	}

	unsigned char buff[50000];
	std::cout << "Entering robot loop" << std::endl;
	while (1)
	{
		memset(buff, 0, 50000 * sizeof(char));
		if ((rob_recv_len = recvfrom(rob_s, (char*)&buff, sizeof(char) * 50000, 0, (struct sockaddr *) &rob_si_other, &rob_slen)) == -1)
		{

			continue;
		}


		int returnval = robot.fillData(robotdata, (unsigned char*)buff);
		if (returnval == 0)
		{
			processThisRobot();
		}


	}
}

