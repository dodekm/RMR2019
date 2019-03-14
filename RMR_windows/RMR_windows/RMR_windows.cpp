// RMR_windows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "RMR_windows.h"


int main()
{
	
	RobotControll robot1;

	std::string command;

	while (1)
	{

		std::getline(std::cin, command);
		
	 if (command == "go")
	{
		float x, y;
		std::cin >> x;
		std::cin >> y;
		robot1.addPoint(RobotPosition(x, y));
		
		robot1.set_start_target(robot1.get_position().coordinates, Point{ x,y });
		
	 }
	 else  if(command!="")
		robot1.set_command(command);
		
	}

	robot1.robotthreadHandle.std::thread::join();
	robot1.laserthreadHandle.std::thread::join();
}




RobotControll::RobotControll() :

	regulator(500, 1),
	mapa(100, 100, -10.0, 10.0, -10.0, 10.0,"file2.txt")
{
	command = "stop";
	command_old = "stop";
	
	odometria_using = &odometria_3;

	WinSock_setup();
	std::cout << "Zadaj IP adresu:" << std::endl;
	
	{
		std::string str;
		std::cin >> str;

		if (str.length()==12)
		ipaddress=str;
	}

	path.push(RobotPosition(0.0, 0.0));

	start = Point{ -2, -1 };
	target = Point{ 7, 7 };
	//start = Point{ -8, -8 };
	//target = Point{ 7, 7 };

	mapa_flood_fill=mapa;
	mapa_flood_fill.FloodFill_fill(start, target,true);
	mapa_flood_fill.saveMap("floodfill.txt");
	mapa_flood_fill.FloodFill_find_path(start, target,floodfill_priority_Y,path); 

	start_threads();
	
}


RobotControll::~RobotControll()
{

}

void RobotControll::WinSock_setup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
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
		printData();
	
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
			motors_speed.translation_speed=20;
			motors_speed.radius = d*100;
			
		}
		else if (command == "right")
		{
			
			motors_speed.translation_speed=20;
			motors_speed.radius = -d*100;
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
			mapa_flood_fill = mapa;
			mapa_flood_fill.FloodFill_fill(start, target,true);
			mapa_flood_fill.FloodFill_find_path(start, target, floodfill_priority_X,path);
			mapa_flood_fill.saveMap("floodfill.txt");
			command_reset();
		}

		else if (command == "reset")
		{
			reset_robot();
			
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
	encoder_init_values(&encL, robotdata.EncoderLeft);
	encoder_init_values(&encR, robotdata.EncoderRight);
	odometria_using->odometry_init();
	motors_speed.translation_speed = 0;
	motors_speed.radius = max_radius;
	command == "stop";
}

void RobotControll::set_start_target(Point start, Point target)
{
	this->start = start;
	this->target = target;
}


RobotPosition RobotControll::get_position()
{
	return actual_position;
}

void RobotControll::printData()
{
	std::cout << "EncoderDataLeft=" << robotdata.EncoderLeft << "tick" << std::endl;
	std::cout << "EncoderDataRight=" << robotdata.EncoderRight << "tick" << std::endl;

	std::cout << "EncoderRealLeft=" << encL.encoder_real_value << "tick" << std::endl;
	std::cout << "EncoderRealRight=" << encR.encoder_real_value << "tick" << std::endl;

	std::cout << "Position X_trapezoidal=" << odometria_3.position.coordinates.X << "m" << std::endl;
	std::cout << "Position Y_trapezoidal=" << odometria_3.position.coordinates.Y << "m" << std::endl;
	std::cout << "Angle_trapezoidal=" << (odometria_3.position.alfa * 180 / PI) << "deg." << std::endl;

	std::cout << "Position X_curved=" << odometria_4.position.coordinates.X << "m" << std::endl;
	std::cout << "Position Y_curved=" << odometria_4.position.coordinates.Y << "m" << std::endl;
	std::cout << "Angle_curved=" << (odometria_4.position.alfa * 180 / PI) << "deg." << std::endl;

	std::cout << "Robot_Mode=" << command << std::endl;


	std::cout << "Position X_wanted=" << wanted_position.coordinates.X << "m" << std::endl;
	std::cout << "Position Y_wanted=" << wanted_position.coordinates.Y << "m" << std::endl;

	std::cout << "vt=" << regulator.getTranslation_output() << "mm/s" << std::endl;
	std::cout << "R=" << regulator.getRotation_output() << "mm" << std::endl;


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
				if(lidar_check_measure(copyOfLaserData.Data[i]))
				mapa.addObstacle(lidar_measure_2_point(copyOfLaserData.Data[i], actual_position));
	}
}

void RobotControll::encoders_process()
{
	encoder_process(&encL, robotdata.EncoderLeft);
	encoder_process(&encR, robotdata.EncoderRight);

}


void RobotControll::processThisLidar(LaserMeasurement &laserData)
{
	std::cout << "Processing Lidar" << std::endl;
	memcpy(&copyOfLaserData, &laserData, sizeof(LaserMeasurement));
	
	if (motors_speed.translation_speed<20.0)
	build_map();
}


void RobotControll::start_threads()
{


	robotthreadHandle = std::thread(robotUDPVlakno, (void *)this);
	std::cout << "Thread1 Started" << std::endl;
	laserthreadHandle = std::thread(laserUDPVlakno, (void *)this);
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
		std::cout << "robot connect error" << std::endl;

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

