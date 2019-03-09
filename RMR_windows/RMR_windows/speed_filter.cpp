#include "speed_filter.h"
#include "pch.h"



int Speed_filter::set_speed(int desired_speed, int n_steps)
{
	if (desired_speed != this->desired_speed)
	{
		this->desired_speed = desired_speed;
		step_size = (desired_speed - actual_speed) / n_steps;
	}

	return step();
}

int Speed_filter::step()
{

	if (abs(actual_speed - desired_speed) > abs(step_size))
		actual_speed += step_size;
	else
		actual_speed = desired_speed;

	return actual_speed;

}