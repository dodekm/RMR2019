#include "regulator.h"
#include "pch.h"


void RobotRegulator::regulate(RobotPosition current_position, RobotPosition desired_position)
{
	float alfa;
	output.translation_speed = -translation_gain * ((current_position.coordinates.X - desired_position.coordinates.X)*cos(current_position.alfa) + (current_position.coordinates.Y - desired_position.coordinates.Y)*sin(current_position.alfa));
	alfa = PointAngle(current_position.coordinates - desired_position.coordinates);
	
	error.X = -cos(alfa - current_position.alfa)*sign(output.translation_speed);
	error.Y = -sin(alfa - current_position.alfa)*sign(output.translation_speed);
	
	delta = PointAngle(error);
	singulatiry_correct(current_position.coordinates,desired_position.coordinates);
	output.radius = rotation_gain / delta * output.translation_speed;
	
	saturate_radius();
	saturate_speed();
	nonlinear_power_function();
}

int RobotRegulator::isRegulated(RobotPosition current_position, RobotPosition desired_position)
{
	return (PointsDistance(current_position.coordinates, desired_position.coordinates) < position_deadzone);
}


template <typename T>int sign(T x)
{
	return (((x) < 0) ? -1 : ((x) > 0));
}