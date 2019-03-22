#pragma once
#include <cmath>
#include "points.h"
#include "encoder.h"

#define tickToMeter  0.000085292090497737556558
#define d  0.23

#define arc_line_switch_treshold 0.0001

class Odometry
{
private:
	float wheel_distance_right = 0;
	float wheel_distance_left = 0;
	float wheel_last_distance_right = 0;
	float wheel_last_distance_left = 0;
	float delta_alfa = 0;
	float delta_l = 0;
	float delta_l_left = 0;
	float delta_l_right = 0;

public:
	RobotPosition position;

	Odometry()
	{
		odometry_init();
	}
	~Odometry()
	{

	}

	void odometry_init();

	void odometry_backward_euler(Encoder encL, Encoder encR);
	void odometry_forward_euler(Encoder encL, Encoder encR);
	void odometry_trapezoidal_rule(Encoder encL, Encoder encR);
	void odometry_curved(Encoder encL, Encoder encR);
};




