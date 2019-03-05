#include "regulator.h"
#include "pch.h"


void RobotRegulator::regulate(Position robot_position, Position desired_position)
{

	translation_output = -translation_gain * ((robot_position.coordinates.X - desired_position.coordinates.X)*cos(robot_position.alfa) + (robot_position.coordinates.Y - desired_position.coordinates.Y)*sin(robot_position.alfa));

	translation_output = 10*sign(translation_output)*sqrt(abs(translation_output));
	alfa = atan2((robot_position.coordinates.Y - desired_position.coordinates.Y), (robot_position.coordinates.X - desired_position.coordinates.X));

	x_error = -cos(alfa - robot_position.alfa)*sign(translation_output);
	y_error = -sin(alfa - robot_position.alfa)*sign(translation_output);
	delta = atan2(y_error, x_error);
	rotation_output = rotation_gain / delta * sign(translation_output);

	saturate_radius();
	saturate();
}



int sign(float x)
{
	return (((x) < 0) ? -1 : ((x) > 0));
}