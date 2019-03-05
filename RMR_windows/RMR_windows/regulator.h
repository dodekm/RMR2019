#pragma once
#define max_speed  250
#define max_radius  65534/2

int sign(float x);

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


	void regulate(Position robot_position, Position desired_position);



	float getTranslation_output()
	{
		return output.translation_speed;
	}

	float getRotation_output()
	{
		return output.radius;
	}

	float position_deadzone = 0.05;
	bool square_root_speed_enable=true;
	float square_root_gain = 15;

	robotSpeed output;

private:
	float translation_gain;
	float rotation_gain;

	
	float x_error;
	float y_error;
	float delta;
	float alfa;
	

	void saturate_radius()
	{
		if (abs(output.radius) > max_radius)
			output.radius = sign(output.radius)*max_radius;
		
	}

	void saturate()
	{
		if (output.translation_speed > max_speed)
		{
			output.translation_speed = max_speed;
		}

		else if (output.translation_speed < -max_speed)
		{
			output.translation_speed = -max_speed;
		}
	}

};