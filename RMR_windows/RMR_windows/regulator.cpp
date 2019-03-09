#include "regulator.h"
#include "pch.h"


void RobotRegulator::regulate(RobotPosition current_position, RobotPosition desired_position)
{
	float delta;
	float alfa;
	output.translation_speed = -translation_gain * ((current_position.coordinates.X - desired_position.coordinates.X)*cos(current_position.alfa) + (current_position.coordinates.Y - desired_position.coordinates.Y)*sin(current_position.alfa));

	if(square_root_speed_enable==true)
		output.translation_speed = square_root_gain*sign(output.translation_speed)*sqrt(abs(output.translation_speed));
	
	alfa = atan2((current_position.coordinates.Y - desired_position.coordinates.Y), (current_position.coordinates.X - desired_position.coordinates.X));

	error.X = -cos(alfa - current_position.alfa)*sign(output.translation_speed);
	error.Y = -sin(alfa - current_position.alfa)*sign(output.translation_speed);
	
	delta = PointAngle(error);
	output.radius = rotation_gain / delta * sign(output.translation_speed);

	saturate_radius();
	saturate_speed();
}

int RobotRegulator::isRegulated(RobotPosition current_position, RobotPosition desired_position)
{
	return (PointsDistance(current_position.coordinates, desired_position.coordinates) < position_deadzone);
}


template <typename T>int sign(T x)
{
	return (((x) < 0) ? -1 : ((x) > 0));
}