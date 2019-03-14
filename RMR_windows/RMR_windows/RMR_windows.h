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
#include <queue> 

#include "CKobuki.h"
#include "rplidar.h"
#include "points.h"
#include "speed_filter.h"
#include "regulator.h"
#include "encoder.h"
#include "map.h"
#include "odometry.h"

#define debug 

#define encoder_max_value  65535 
#define arc_line_switch_treshold 0.0001
#define tickToMeter  0.000085292090497737556558
#define d  0.23





class RobotControll
{
public:
	
	RobotControll();
	~RobotControll();
	void WinSock_setup();
	void robotprocess();
	void laserprocess();
	void encoders_process();

	void processThisLidar(LaserMeasurement &laserData);
	void processThisRobot();
	void start_threads();
	void automode();
	void build_map();

	void printData();
	void reset_robot();
	
	void move_arc(int mmpersec, int radius);
	void forward(int);
	void back(int);
	void left(double);
	void right(double);
	void stop();
	
	void set_start_target(Point start, Point target);
	RobotPosition get_position();
	void addPoint(RobotPosition P)
	{
		path.push(P);
	}

	void set_command(std::string command)
	{
		this->command_old = this->command;
		this->command = command;
	}
	std::string get_command()
	{
		return command;
	}

	void command_reset()
	{
		command = command_old;
	}



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

private:

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

	LaserMeasurement copyOfLaserData;
	std::string ipaddress= "192.168.1.15";
	CKobuki robot;
	TKobukiData robotdata;
	
	unsigned int datacounter=0;
	int modulo_print = 50;
	int modulo_odometry = 10;
	int modulo_drive = 5;

	Encoder encL;
	Encoder encR;
	Odometry odometria_1;
	Odometry odometria_2;
	Odometry odometria_3;
	Odometry odometria_4;
	Odometry* odometria_using;
	
	
	robotSpeed motors_speed{ 0,0 };
	
	RobotPosition actual_position;
	RobotPosition wanted_position;
	Point start;
	Point target;

	std::queue <RobotPosition> path;

	int speed_filter_steps = 10;
	Speed_filter filter;

	Mapa mapa;
	Mapa mapa_flood_fill;

	RobotRegulator regulator;
	
	std::string command;
	std::string command_old;	

};



