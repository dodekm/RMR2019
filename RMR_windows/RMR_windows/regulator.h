#pragma once
#define max_speed  250
#define max_radius  65534/2

int sign(float x);

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
	void regulate(Position robot_position, Position desired_position);



	float getTranslation_output()
	{
		return translation_output;
	}

	float getRotation_output()
	{
		return rotation_output;
	}
	float position_deadzone = 0.05;

private:
	float translation_gain;
	float rotation_gain;
	float translation_output;
	float rotation_output;
	float x_error;
	float y_error;
	float delta;
	float alfa;

	void saturate_radius()
	{
		if (abs(rotation_output) > max_radius)
			rotation_output = sign(rotation_output)*max_radius;
		
	}

	void saturate()
	{
		if (translation_output > max_speed)
		{
			translation_output = max_speed;
		}

		else if (translation_output < -max_speed)
		{
			translation_output = -max_speed;
		}
	}

};