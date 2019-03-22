#pragma once


#define _USE_MATH_DEFINES

#include<iostream>
#include <windows.h>
#include <winsock.h>

#include<vector>
#include <queue> 
#include<string>
#include <ostream>
#include<thread>
#include "CKobuki.h"
#include "rplidar.h"
#include "points.h"
#include "speed_filter.h"
#include "regulator.h"
#include "encoder.h"
#include "map.h"
#include "odometry.h"

#define debug 

#define lidar_measure_modulo 5000
#define histogram_treshold 20


class RobotControll
{
public:
	
	RobotControll();
	~RobotControll();

	///Interface
	void set_start(Point start);
	void set_target(Point target);
	RobotPosition get_position();
	RobotPosition get_wanted_position();
	robotSpeed get_motors_speed();
	Point get_target_point();
	Point get_starting_point();
	std::vector<RobotPosition>get_path();
	void set_command(std::string command);
	std::string get_command();
	void command_reset();
	void addPointToPath(RobotPosition P);
	Mapa getMap();
	

	friend std::ostream& operator<<(std::ostream& stream, RobotControll& robot)
	{
		robot.printData(stream);
		return stream;
	}
	
	void WinSock_setup();
	void robotprocess();
	void laserprocess();
	void encoders_process();

	void processThisLidar(LaserMeasurement &laserData);
	void processThisRobot();
	void start_threads();
	void automode();
	void build_map();
	void find_path();

	void printData(std::ostream& stream);
	void reset_robot();
	
	void move_arc(int mmpersec, int radius);
	void forward(int);
	void back(int);
	void left(double);
	void right(double);
	void stop();
	
	
	std::thread robotthreadHandle; 
	std::thread laserthreadHandle; 
	
	void setip(std::string ip);
	void setfilename(std::string filename);

	void* parent_ptr = NULL;
	void(*parent_robot_update_callback)(void*)=NULL;
	void(*parent_map_update_callback)(void*)=NULL;
	
	void emit_odometry()
	{
		if (parent_robot_update_callback)
			parent_robot_update_callback(parent_ptr);
	}
	void emit_map()
	{
		if (parent_map_update_callback)
			parent_map_update_callback(parent_ptr);
	}
	
	static void robotUDPVlakno(void *param)
	{
		((RobotControll*)param)->robotprocess();
		return;
	}
	
	
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
	std::string ipaddress= "192.168.1.13";
	
	CKobuki robot;
	TKobukiData robotdata;
	
	unsigned long datacounter=0;
	unsigned long lidar_measure_counter = 0;

	int modulo_print = 50;
	int modulo_odometry = 1;
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
	Mapa histogram;

	RobotRegulator regulator;
	
	std::string filename="";
	std::string command;
	std::string command_old;	

};



