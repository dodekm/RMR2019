#pragma once

#include <cmath>
#include "points.h"
#include "misc.h"

#define max_speed  450
#define min_speed  30

#define max_radius 65534/2

#define min_radius  70

#define singularity_product  2


typedef struct robotSpeed
{
	float translation_speed;
	float radius;
};

class RobotRegulator
{
public:
	bool enabled = false;
	RobotRegulator()
	{

	}

	RobotRegulator(float translation_gain, float rotation_gain)
	{
		this->translation_gain = translation_gain;
		this->rotation_gain = rotation_gain;
	}



	~RobotRegulator()
	{

	}

	void regulate(RobotPosition& current_position, RobotPosition& desired_position);
	void regulate_alternative(RobotPosition& current_position, RobotPosition& desired_position);

	int isRegulated(RobotPosition& current_position, RobotPosition& desired_position);


	float getTranslation_output()
	{
		return output.translation_speed;
	}

	float getRotation_output()
	{
		return output.radius;
	}

	float position_deadzone = 0.06;

	robotSpeed output;

private:
	float translation_gain;
	float rotation_gain;
	float gamma = 0.6;
	float delta;
	Point error;

	void saturate_radius()
	{
		if (abs(output.radius) > max_radius)
		{
			output.radius = sign(output.radius)*max_radius;
		}
		if (abs(output.radius) < min_radius)
		{
			output.radius = sign(output.radius)*min_radius;
		}
	}


	void saturate_speed()
	{
		if (abs(output.translation_speed) > max_speed)
		{
			output.translation_speed = sign(output.translation_speed)*max_speed;
		}
		if (abs(output.translation_speed) < min_speed)
		{
			output.translation_speed = 0;
		}
	}



	void speed_rotation_correction()
	{
	
		float k = 10;

		float l = abs(output.radius) / max_radius;
		output.translation_speed = output.translation_speed* (1+k*l)/(1+k);

	}

	void nonlinear_power_function()
	{
		float x = abs(output.translation_speed/max_speed);
		output.translation_speed = max_speed * (pow(x, gamma))*sign(output.translation_speed);
	}


	void symmetry_correct(Point current_position, Point desired_position)
	{
		if (abs(output.translation_speed) < min_speed)
		{
			if (PointLength(desired_position - current_position)>position_deadzone)
			{
				output.translation_speed = sign(output.translation_speed)*min_speed * singularity_product;

			}
		}
	}

};