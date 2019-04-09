
#include "regulator.h"



void RobotRegulator::regulate(RobotPosition& current_position, RobotPosition& desired_position)
{
	
	float alfa;
	output.translation_speed = -translation_gain * ((current_position.coordinates.X - desired_position.coordinates.X)*cos(current_position.alfa) + (current_position.coordinates.Y - desired_position.coordinates.Y)*sin(current_position.alfa));
	alfa = Point_angle(current_position.coordinates - desired_position.coordinates);
	
	error.X = -cos(alfa - current_position.alfa)*sign(output.translation_speed);
	error.Y = -sin(alfa - current_position.alfa)*sign(output.translation_speed);
	
	delta = Point_angle(error);
	symmetry_correct(current_position.coordinates,desired_position.coordinates);
	output.radius = rotation_gain / delta * output.translation_speed;
	
	saturate_radius();
	saturate_speed();
	nonlinear_power_function();
}


void RobotRegulator::regulate_alternative(RobotPosition& current_position, RobotPosition& desired_position)
{
	
	output.translation_speed = -translation_gain * ((current_position.coordinates.X - desired_position.coordinates.X)*cos(current_position.alfa) + (current_position.coordinates.Y - desired_position.coordinates.Y)*sin(current_position.alfa));

	delta = ((current_position.coordinates.X - desired_position.coordinates.X)*sin(current_position.alfa) - (current_position.coordinates.Y - desired_position.coordinates.Y)*cos(current_position.alfa));
	//delta /= (current_position.coordinates.X - desired_position.coordinates.X)*(current_position.coordinates.X - desired_position.coordinates.X) + (current_position.coordinates.Y - desired_position.coordinates.Y)*(current_position.coordinates.Y - desired_position.coordinates.Y);
	output.radius = rotation_gain * abs(output.translation_speed) / delta;

	symmetry_correct(current_position.coordinates, desired_position.coordinates);
	
	saturate_radius();
	saturate_speed();
	nonlinear_power_function();
}


int RobotRegulator::isRegulated(RobotPosition& current_position, RobotPosition& desired_position)
{
	return (PointsDistance(current_position.coordinates, desired_position.coordinates) < position_deadzone);
}


