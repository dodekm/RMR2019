#include "regulator.h"
#include "pch.h"


void RobotRegulator::regulate(Position robot_position, Position desired_position)
{

	output.translation_speed = -translation_gain * ((robot_position.coordinates.X - desired_position.coordinates.X)*cos(robot_position.alfa) + (robot_position.coordinates.Y - desired_position.coordinates.Y)*sin(robot_position.alfa));

	if(square_root_speed_enable==true)
		output.translation_speed = square_root_gain*sign(output.translation_speed)*sqrt(abs(output.translation_speed));
	
	alfa = atan2((robot_position.coordinates.Y - desired_position.coordinates.Y), (robot_position.coordinates.X - desired_position.coordinates.X));

	x_error = -cos(alfa - robot_position.alfa)*sign(output.translation_speed);
	y_error = -sin(alfa - robot_position.alfa)*sign(output.translation_speed);
	delta = atan2(y_error, x_error);
	output.radius = rotation_gain / delta * sign(output.translation_speed);

	saturate_radius();
	saturate();
}



int sign(float x)
{
	return (((x) < 0) ? -1 : ((x) > 0));
}