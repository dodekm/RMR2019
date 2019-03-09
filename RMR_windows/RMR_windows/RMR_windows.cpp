// RMR_windows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "RMR_windows.h"


int main()
{
	std::cout << "APP START" << std::endl;
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
		
		
		
	 }
	 else  if(command!="")
		robot1.set_command(command);
		
	}

	robot1.robotthreadHandle.std::thread::join();
	robot1.laserthreadHandle.std::thread::join();
}

void odometry_init(Odometry* odometria)
{

	odometria->wheel_distance_right = 0;
	odometria->wheel_distance_left = 0;
	odometria->wheel_last_distance_right = 0;
	odometria->wheel_last_distance_left = 0;
	odometria->delta_alfa = 0;
	odometria->delta_l = 0;
	odometria->delta_l_left = 0;
	odometria->delta_l_right = 0;
	odometria->position = RobotPosition{0,0};


}


RobotControll::RobotControll():

	
regulator(200, 100),
mapa(100, 100, -5.0, 5.0, -5.0, 5.0)
{
	command = "stop";
	command_old = "stop";
	
	odometry_init(&odometria_1);
	odometry_init(&odometria_2);
	odometry_init(&odometria_3);
	odometry_init(&odometria_4);

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
		odometry_forward_euler(&(odometria_1));
		odometry_backward_euler(&(odometria_2));
		odometry_trapezoidal_rule(&(odometria_3));
		odometry_curved(&(odometria_4));

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
			motors_speed.translation_speed=150;
			motors_speed.radius = max_radius;
		}
		else if (command == "back")
		{
			motors_speed.translation_speed=-150;
			motors_speed.radius = max_radius;
		}
		else if (command == "left")
		{
			motors_speed.translation_speed=10;
			motors_speed.radius = 10;
			
		}
		else if (command == "right")
		{
			
			motors_speed.translation_speed=10;
			motors_speed.radius = -10;
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
	odometry_init(odometria_using);
	motors_speed.translation_speed = 0;
	motors_speed.radius = max_radius;
	command == "stop";
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
				mapa.addPoint(lidar_measure_2_point(copyOfLaserData.Data[i], actual_position));
	}
}

void RobotControll::encoders_process()
{
	encoder_process(&encL, robotdata.EncoderLeft);
	encoder_process(&encR, robotdata.EncoderRight);

}

void RobotControll::odometry_backward_euler(Odometry* odometria)
{
	odometria->wheel_distance_left = encL.encoder_real_value*tickToMeter;
	odometria->wheel_distance_right = encR.encoder_real_value*tickToMeter;


	odometria->delta_l_left = odometria->wheel_distance_left - odometria->wheel_last_distance_left;
	odometria->delta_l_right = odometria->wheel_distance_right - odometria->wheel_last_distance_right;


	odometria->delta_l = (odometria->delta_l_left + odometria->delta_l_right) / 2;

	odometria->position.coordinates.X += odometria->delta_l * cos(odometria->position.alfa);
	odometria->position.coordinates.Y += odometria->delta_l * sin(odometria->position.alfa);

	odometria->delta_alfa = -(odometria->delta_l_left - odometria->delta_l_right) / d;
	odometria->position.alfa += odometria->delta_alfa;

	odometria->wheel_last_distance_left = odometria->wheel_distance_left;
	odometria->wheel_last_distance_right = odometria->wheel_distance_right;

}

void RobotControll::odometry_forward_euler(Odometry* odometria)
{
	odometria->wheel_distance_left = encL.encoder_real_value*tickToMeter;
	odometria->wheel_distance_right = encR.encoder_real_value*tickToMeter;

	odometria->position.coordinates.X += odometria->delta_l * cos(odometria->position.alfa);
	odometria->position.coordinates.Y += odometria->delta_l * sin(odometria->position.alfa);

	odometria->position.alfa += odometria->delta_alfa;

	odometria->delta_l_left = odometria->wheel_distance_left - odometria->wheel_last_distance_left;
	odometria->delta_l_right = odometria->wheel_distance_right - odometria->wheel_last_distance_right;


	odometria->delta_l = (odometria->delta_l_left + odometria->delta_l_right) / 2;
	odometria->delta_alfa = -(odometria->delta_l_left - odometria->delta_l_right) / d;


	odometria->wheel_last_distance_left = odometria->wheel_distance_left;
	odometria->wheel_last_distance_right = odometria->wheel_distance_right;

}


void RobotControll::odometry_trapezoidal_rule(Odometry* odometria)
{
	odometria->wheel_distance_left = encL.encoder_real_value*tickToMeter;
	odometria->wheel_distance_right = encR.encoder_real_value*tickToMeter;


	odometria->delta_l_left = odometria->wheel_distance_left - odometria->wheel_last_distance_left;
	odometria->delta_l_right = odometria->wheel_distance_right - odometria->wheel_last_distance_right;

	float delta_l_old = odometria->delta_l;
	float delta_alfa_old = odometria->delta_alfa;


	odometria->delta_l = (odometria->delta_l_left + odometria->delta_l_right) / 2;
	odometria->position.coordinates.X += ((odometria->delta_l + delta_l_old) / 2) * cos(odometria->position.alfa);
	odometria->position.coordinates.Y += ((odometria->delta_l + delta_l_old) / 2)* sin(odometria->position.alfa);


	odometria->delta_alfa = -(odometria->delta_l_left - odometria->delta_l_right) / d;
	odometria->position.alfa += ((odometria->delta_alfa + delta_alfa_old) / 2);


	odometria->wheel_last_distance_left = odometria->wheel_distance_left;
	odometria->wheel_last_distance_right = odometria->wheel_distance_right;

}




void RobotControll::odometry_curved(Odometry* odometria)
{

	odometria->wheel_distance_left = encL.encoder_real_value*tickToMeter;
	odometria->wheel_distance_right = encR.encoder_real_value*tickToMeter;

	odometria->delta_l_left = odometria->wheel_distance_left - odometria->wheel_last_distance_left;
	odometria->delta_l_right = odometria->wheel_distance_right - odometria->wheel_last_distance_right;
	odometria->delta_l = (odometria->delta_l_left + odometria->delta_l_right)/2;

	odometria->delta_alfa = -(odometria->delta_l_left - odometria->delta_l_right) / d;
	if (abs(odometria->delta_l_right - odometria->delta_l_left) > arc_line_switch_treshold)
	{
		odometria->position.coordinates.X += (d*(odometria->delta_l_left + odometria->delta_l_right) / (odometria->delta_l_right - odometria->delta_l_left) / 2 * (sin(odometria->position.alfa + odometria->delta_alfa) - sin(odometria->position.alfa)));
		odometria->position.coordinates.Y += (d*(odometria->delta_l_left + odometria->delta_l_right) / (odometria->delta_l_right - odometria->delta_l_left) / 2 * (cos(odometria->position.alfa + odometria->delta_alfa) - cos(odometria->position.alfa)));
	}
	else
	{
		odometria->position.coordinates.X += odometria->delta_l * cos(odometria->position.alfa);
		odometria->position.coordinates.Y += odometria->delta_l * sin(odometria->position.alfa);
	}


	odometria->position.alfa += odometria->delta_alfa;

	odometria->wheel_last_distance_left = odometria->wheel_distance_left;
	odometria->wheel_last_distance_right = odometria->wheel_distance_right;

}



void RobotControll::processThisLidar(LaserMeasurement &laserData)
{
	std::cout << "Processing Lidar" << std::endl;
	memcpy(&copyOfLaserData, &laserData, sizeof(LaserMeasurement));
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

