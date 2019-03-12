#pragma once
 
#include <cmath>

#define max_speed  350
#define min_speed  30

#define max_radius  65534/2
#define min_radius  350

template <typename T>int sign(T x);

typedef struct robotSpeed
{
	float translation_speed;
	float radius;
};

class RobotRegulator
{
public:
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

	void regulate(RobotPosition current_position, RobotPosition desired_position);
	int isRegulated(RobotPosition current_position, RobotPosition desired_position);


	float getTranslation_output()
	{
		return output.translation_speed;
	}

	float getRotation_output()
	{
		return output.radius;
	}

	float position_deadzone = 0.05;

	robotSpeed output;

private:
	float translation_gain;
	float rotation_gain;
	float gamma = 0.5;

	Point error;

	void saturate_radius()
	{
		if (abs(output.radius) > max_radius)
			output.radius = sign(output.radius)*max_radius;

		if (abs(output.radius) < min_radius)
			output.radius = sign(output.radius)*min_radius;
		
	}


	void saturate_speed()
	{
		if (abs(output.translation_speed) > max_speed)
			output.translation_speed = sign(output.translation_speed)*max_speed;

		if (abs(output.translation_speed) < min_speed)
			output.translation_speed = 0;

	}

	void nonlinear_power_function()
	{
		float x = abs(output.translation_speed/max_speed);
		output.translation_speed = max_speed * (pow(x, gamma))*sign(output.translation_speed);
	}


};