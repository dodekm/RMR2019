#pragma once

#define _USE_MATH_DEFINES
#include <Qobject>

#include<iostream>
#include <windows.h>
#include <winsock.h>

#include<vector>
#include <queue> 
#include<string>
#include <ostream>
#include<thread>
#include <mutex>
#include "CKobuki.h"
#include "rplidar.h"
#include "points.h"
#include "speed_filter.h"
#include "regulator.h"
#include "encoder.h"
#include "map.h"
#include "odometry.h"



#define lidar_measure_modulo 1000
#define histogram_treshold 10

const std::string command_to_string []=
{
	"stop",
	"forward",
	"back",
	"left",
	"right",
	"auto",
	"save",
	"load",
	"find",
	"reset",
	"clear"
};


enum robot_command
{
	stop,
	forward,
	back,
	left,
	right,
	automatic,
	save,
	load,
	find,
	reset,
	clear,
	disconnect,
	print,
	clear_path,

};

typedef struct
{
	RobotPosition actual_position;
	RobotPosition wanted_position;
	robotSpeed motors_speed;
	Point start;
	Point target;
	robot_command command;
	std::string command_string;
	std::vector<RobotPosition> path;
	bool connection_status;

}Robot_feedback;

Q_DECLARE_METATYPE(Robot_feedback);


class RobotControll:public QObject
{
	Q_OBJECT

signals:

	void map_update_sig(Mapa);
	void odometry_update_sig(Robot_feedback);
	
public slots:
	///Interface
	void set_start(Point start);
	void set_target(Point target);

	void set_command(robot_command);
	void addPointToPath(RobotPosition);
	
	void set_threads_enabled(bool status);
	void start_threads();
	void stop_threads();
	void setip(std::string ip);
	void setfilename(std::string );

public:
	
	RobotControll();
	~RobotControll();

	QObject* gui;
	bool connection_status=false;

	void command_reset();
	std::string get_command();
	RobotPosition get_position();
	RobotPosition get_wanted_position();
	robotSpeed get_motors_speed();
	Point get_target_point();
	Point get_starting_point();
	Robot_feedback RobotControll::getRobotData();
	

	std::vector<RobotPosition>get_path();

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
	void robot_controll();

	void automode();
	void build_map();
	void find_path();

	void printData(std::ostream& stream);
	void reset_robot();
	void clear_path();


	void move_arc(int mmpersec, int radius);
	void forward(int);
	void back(int);
	void left(double);
	void right(double);
	void stop();
	
	
	std::thread robotthreadHandle; 
	std::thread laserthreadHandle; 
	std::thread controllhreadHandle;

	std::mutex mutex_robot;
	std::mutex mutex_map;

	

private:

	bool threads_enabled = true;
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

	Speed_filter filter;

	Mapa mapa;
	Mapa histogram;
	Mapa map_with_path;
	


	RobotRegulator regulator;
	
	std::string filename="";

	robot_command command= robot_command::stop;
	robot_command command_old= robot_command::stop;

};





