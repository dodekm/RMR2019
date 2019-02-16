#pragma once
//#include <QMainWindow>
//#include<QMutex>
#include<iostream>
//#include<arpa/inet.h>
//#include<unistd.h>
//#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include "CKobuki.h"
#include "rplidar.h"


class RobotControll
{
public:

	void robotprocess();
	void laserprocess();
	void processThisLidar(LaserMeasurement &laserData);
	void processThisRobot();
	void start_threads();
	void forward();
	void back();
	void left();
	void right();
	void stop();

	pthread_t robotthreadHandle; // handle na vlakno
	int robotthreadID;  // id vlakna

	static void *robotUDPVlakno(void *param)
	{
		((RobotControll*)param)->robotprocess();
		return 0;
	}
	
	pthread_t laserthreadHandle; // handle na vlakno
	int laserthreadID;  // id vlakna
	
	static void *laserUDPVlakno(void *param)
	{
		((RobotControll*)param)->laserprocess();
		return 0;
	}
	//veci na broadcast laser
	struct sockaddr_in las_si_me, las_si_other, las_si_posli;

	int las_s, las_recv_len;
	unsigned int las_slen;
	//veci na broadcast robot
	struct sockaddr_in rob_si_me, rob_si_other, rob_si_posli;

	int rob_s, rob_recv_len;
	unsigned int rob_slen;


private:

	LaserMeasurement copyOfLaserData;
	std::string ipaddress;
	CKobuki robot;
	TKobukiData robotdata;
	int datacounter;

};