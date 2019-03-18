#pragma once
#include <cmath>

class Speed_filter
{

public:
	Speed_filter()
	{
		actual_speed = 0;
		desired_speed = 0;
		step_size = 0;
	}
	~Speed_filter()
	{

	}
	int set_speed(int desired_speed, int n_steps);
	int step();

private:

	int actual_speed;
	int desired_speed;
	int step_size;
	int n_steps;
};

