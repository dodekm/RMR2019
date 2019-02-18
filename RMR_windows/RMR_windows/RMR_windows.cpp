// RMR_windows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "RMR_windows.h"


int main()
{
	std::cout << "APP START" << std::endl;
	RobotControll robot1;
	
	
	robot1.robotthreadHandle.std::thread::join();
	robot1.laserthreadHandle.std::thread::join();
}


RobotControll::RobotControll()
{
	WinSock_setup();
	std::cout << "Zadaj IP adresu:" << std::endl;
	std::cin >> ipaddress;

	datacounter = 0;
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


void encoder_process(Encoder* enc, uint16_t encoder_data)
{
	enc->encoder_current_value = encoder_data;
	
	if ((enc->encoder_current_value - enc->encoder_last_value) < -encoder_max_value / 2)
		enc->owerflow_flounter++;

	else if ((enc->encoder_current_value - enc->encoder_last_value) > encoder_max_value / 2)
		enc->owerflow_flounter--;

	enc->encoder_real_value = encoder_max_value * enc->owerflow_flounter + enc->encoder_current_value;

	enc->encoder_last_value = enc->encoder_current_value;
	
}

void RobotControll::processThisRobot()
{


		std::cout << "Processing Robot" << std::endl;
		std::cout<<"EncoderLeft" << robotdata.EncoderLeft << std::endl; 
		std::cout << "EncoderRight" << robotdata.EncoderRight << std::endl;
		std::cout << "EncoderLeft" << robotdata.EncoderLeft << std::endl;
		std::cout << "EncoderRight" << robotdata.EncoderRight << std::endl;
		std::cout << "Bumper Center" << robotdata.BumperCenter << std::endl;
		std::cout << "Bumper Left" << robotdata.BumperLeft << std::endl;
		std::cout << "Bumper Right" << robotdata.BumperRight << std::endl;
		
		encoders_process();
		odometry();

	datacounter++;
}


void RobotControll::encoders_process()
{
	encoder_process(&encL,robotdata.EncoderLeft);
	encoder_process(&encR,robotdata.EncoderRight);

}

void RobotControll::odometry()
{
	odometria.wheel_distance_left = encL.encoder_current_value*tickToMeter;
	odometria.wheel_distance_right = encR.encoder_current_value*tickToMeter;
	
	float delta_l_left, delta_l_right;
	
	delta_l_left = odometria.wheel_distance_left - odometria.wheel_last_distance_left;
	delta_l_right = odometria.wheel_distance_right - odometria.wheel_last_distance_right;

	float delta_l;
	delta_l = (delta_l_left + delta_l_right) / 2;

	float delta_alfa;
	delta_alfa = (delta_l_left - delta_l_right) / d;
	
	odometria.x_pos = +delta_l* (cos(odometria.alfa)+ cos(odometria.alfa+delta_alfa))/2;
	odometria.y_pos = +delta_l* (sin(odometria.alfa)+ sin(odometria.alfa+delta_alfa))/2;

	odometria.alfa = +delta_alfa;
	odometria.wheel_last_distance_left = odometria.wheel_distance_left;
	odometria.wheel_last_distance_right =odometria.wheel_distance_right;


}

void RobotControll::odometry_curved()
{



}

void RobotControll::processThisLidar(LaserMeasurement &laserData)
{
	std::cout << "Processing Lidar" << std::endl;
	memcpy(&copyOfLaserData, &laserData, sizeof(LaserMeasurement));

	
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

	std::vector<unsigned char> mess=robot.setArcSpeed(mmpersec,radius);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}

void RobotControll::forward(int mmpersec) //forward
{
	//pohyb dopredu
	std::vector<unsigned char> mess = robot.setTranslationSpeed(mmpersec);
	/// std::vector<unsigned char> mess=robot.setArcSpeed(100,1000);
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
	
	int result=bind(las_s, (struct sockaddr*)&las_si_me, sizeof(las_si_me));
	if (result == SOCKET_ERROR)
		std::cout << "laser connect error" << std::endl;

	char command = 0x00;
	if (sendto(las_s, &command, sizeof(command), 0, (struct sockaddr*) &las_si_posli, las_slen) == -1)
	{
		std::cout << "send error" << std::endl;
	}
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
	
	int result=bind(rob_s, (struct sockaddr*)&rob_si_me, sizeof(rob_si_me));
	if(result==SOCKET_ERROR)
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

