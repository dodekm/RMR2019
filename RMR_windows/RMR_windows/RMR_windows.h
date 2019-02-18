#pragma once
#pragma comment(lib, "ws2_32.lib")
#include<iostream>
#include <windows.h>
#include <winsock.h>

#include<thread>
#include<vector>
#include<string>
#include<chrono>
#include<stdlib.h>

#include "CKobuki.h"
#include "rplidar.h"

#define encoder_max_value  65535 


typedef struct Odometry
{
	float wheel_distance_right=0;
	float wheel_distance_left=0;
	float wheel_last_distance_right=0;
	float wheel_last_distance_left=0;
	float alfa=0;
	float x_pos=0, y_pos=0;
};


typedef struct  Encoder
{
	uint16_t encoder_current_value;
	uint16_t encoder_last_value;
	int encoder_real_value;
	int owerflow_flounter;
	
};


void encoder_process(Encoder, uint16_t);


class RobotControll
{
public:
	

	RobotControll();
	~RobotControll();
	void WinSock_setup();
	void robotprocess();
	void laserprocess();
	void encoders_process();
	void odometry();
	void odometry_curved();
	void processThisLidar(LaserMeasurement &laserData);
	void processThisRobot();
	void start_threads();
	void move_arc(int mmpersec, int radius);
	void forward(int);
	void back(int);
	void left(double);
	void right(double);
	void stop();

	std::thread robotthreadHandle; // handle na vlakno
	
	static void robotUDPVlakno(void *param)
	{
		((RobotControll*)param)->robotprocess();
		return;
	}
	
	std::thread laserthreadHandle; // handle na vlakno
	
	static void laserUDPVlakno(void *param)
	{
		((RobotControll*)param)->laserprocess();
		return;
	}


	//veci na broadcast laser
	struct sockaddr_in las_si_me, las_si_other, las_si_posli;

	SOCKET las_s;
	int las_recv_len;
	int las_slen;
	//veci na broadcast robot
	struct sockaddr_in rob_si_me, rob_si_other, rob_si_posli;

	SOCKET rob_s;
	int	rob_recv_len;
	int rob_slen;


private:

	LaserMeasurement copyOfLaserData;
	std::string ipaddress= "192.168.1.13";
	CKobuki robot;
	TKobukiData robotdata;
	int datacounter;
	
	Encoder encL = { 0,0,0,0 };
	Encoder encR = { 0,0,0,0 };
	Odometry odometria;

	const long double tickToMeter = 0.000085292090497737556558; // [m/tick]
	const long double d = 0.23;
	

};