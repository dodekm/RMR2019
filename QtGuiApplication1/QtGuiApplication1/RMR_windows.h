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
#include "slam.h"


#define lidar_build_modulo 1000
#define lidar_scan_modulo 1500

#define slam_modulo_main 20


#define histogram_treshold lidar_build_modulo/50

#define zone_width 0.2
#define point_dist_treshold 0.2


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
	"clear",
	"disconnect",
	"print",
	"clear_path",
	"slam",
	"build_map",
	"build_scope"
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
	slam,
	build_map,
	build_scope

};

typedef struct
{
	RobotPosition actual_position;
	RobotPosition wanted_position;
	RobotPosition wanted_position_corrected;
	std::list<obstacle> obstacles;
	std::list<obstacle> obstacles_in_way;
	RobotPosition slam_position;
	float slam_estimate_quality;
	robotSpeed motors_speed;
	Point start;
	Point target;
	robot_command command;
	std::queue<robot_command> command_queue_send;
	std::string command_string;
	std::vector<RobotPosition> path;

}Robot_feedback;

Q_DECLARE_METATYPE(Robot_feedback);


class RobotControll:public QObject
{
	Q_OBJECT

signals:

	void map_update_sig(Mapa);
	void scope_update_sig(Mapa);
	void odometry_update_sig(Robot_feedback);
	
public slots:
	///Interface
	void set_start(Point start);
	void set_target(Point target);

	void push_command(robot_command);
	robot_command pop_command();
	void reset_command_queue();

	void addPointToPath(RobotPosition);
	
	void set_threads_enabled(bool status);
	void set_maping_enabled(bool status);
	void set_map_with_path_enabled(bool status);
	void set_slam_enabled(bool status);


	void start_threads();
	void stop_threads();
	void setip(std::string ip);
	void setfilename(std::string );

public:
	
	RobotControll();
	~RobotControll();

	QObject* gui_ptr;
	
	std::string get_command_name();
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

	void processThisLidar(std::vector<LaserData> new_scan);
	void processThisRobot();
	void robot_controll();


	void obstacle_avoidance();
	void automode();

	void build_map();
	void build_scope();
	void find_path(Mapa working_map);

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

	std::mutex mutex_robot_data;
	std::mutex mutex_command_queue;
	std::mutex mutex_lidar_data;

	

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

	std::vector<LaserData> Laser_data_working;
	std::vector<LaserData> Laser_data_new;

	std::string ipaddress= "192.168.1.13";
	
	CKobuki robot;
	TKobukiData robotdata;
	
	unsigned long datacounter=0;
	unsigned long slam_counter = 1;
	unsigned long lidar_measure_counter = 0;

	Encoder encL;
	Encoder encR;
	Odometry odometria_1;
	Odometry odometria_2;
	Odometry odometria_3;
	Odometry odometria_4;
	
	
	robotSpeed motors_speed{ 0,0 };
	

	RobotPosition reset_position;

	RobotPosition odometry_position_last;
	RobotPosition odometry_position;

	RobotPosition actual_position;
	RobotPosition slam_position;
	RobotPosition wanted_position;
	RobotPosition wanted_position_corrected;

	Point start;
	Point target;

	std::queue <RobotPosition> path;
	

	Speed_filter filter_speed;
	
	bool maping_enable = false;
	bool map_with_path_enable = false;
	bool slam_enable = true;
	Mapa mapa;
	Mapa histogram;
	Mapa map_with_path;
	Mapa current_scope;

	std::list<Point> current_scope_obstacles;
	bool is_obstacle_in_way(obstacle);
	std::list<obstacle> get_obstacles_in_way(std::list<obstacle>);
	bool is_point_in_way(Point m);
	std::list<obstacle> obstacles;
	std::list<obstacle> find_obstacles(std::list<Point>points);
	std::list<obstacle> obstacles_in_way;
	Slam slam;
	RobotRegulator regulator;
	
	std::string filename="";

	robot_command command= robot_command::stop;
	std::queue<robot_command>command_queue;

};





