
#include "speed_filter.h"
#include "points.h"

int Speed_filter::set_speed(int desired_speed, int n_steps)
{
	if (desired_speed != this->desired_speed)
	{
		this->desired_speed = desired_speed;
		if ((desired_speed - actual_speed) / n_steps != 0)
			step_size = (desired_speed - actual_speed) / n_steps;
		else
			step_size = sign(desired_speed - actual_speed);

	}

	return step();
}

int Speed_filter::step()
{

	if (abs(actual_speed - desired_speed) > abs(step_size))
	{
		actual_speed += step_size;
	}
	else
	{
		actual_speed = desired_speed;
	}
	return actual_speed;

}