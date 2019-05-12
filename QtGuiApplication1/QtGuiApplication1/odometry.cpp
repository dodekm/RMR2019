
#include "odometry.h"

void Odometry::odometry_init(RobotPosition start)
{

	wheel_distance_right = 0;
	wheel_distance_left = 0;
	wheel_last_distance_right = 0;
	wheel_last_distance_left = 0;
	delta_alfa = 0;
	delta_l = 0;
	delta_l_left = 0;
	delta_l_right = 0;
	position = start;

}

void Odometry::odometry_backward_euler(Encoder encL, Encoder encR)
{
	wheel_distance_left = encL.encoder_real_value*tickToMeter;
	wheel_distance_right = encR.encoder_real_value*tickToMeter;


	delta_l_left = wheel_distance_left - wheel_last_distance_left;
	delta_l_right = wheel_distance_right - wheel_last_distance_right;


	delta_l = (delta_l_left + delta_l_right) / 2;

	position.coordinates.X += delta_l * cos(position.alfa);
	position.coordinates.Y += delta_l * sin(position.alfa);

	delta_alfa = -(delta_l_left - delta_l_right) / d;
	position.alfa += delta_alfa;

	wheel_last_distance_left = wheel_distance_left;
	wheel_last_distance_right = wheel_distance_right;

}


void Odometry::odometry_forward_euler(Encoder encL, Encoder encR)
{
	wheel_distance_left = encL.encoder_real_value*tickToMeter;
	wheel_distance_right = encR.encoder_real_value*tickToMeter;

	position.coordinates.X += delta_l * cos(position.alfa);
	position.coordinates.Y += delta_l * sin(position.alfa);

	position.alfa += delta_alfa;

	delta_l_left = wheel_distance_left - wheel_last_distance_left;
	delta_l_right = wheel_distance_right - wheel_last_distance_right;


	delta_l = (delta_l_left + delta_l_right) / 2;
	delta_alfa = -(delta_l_left - delta_l_right) / d;


	wheel_last_distance_left = wheel_distance_left;
	wheel_last_distance_right = wheel_distance_right;

}


void Odometry::odometry_trapezoidal_rule(Encoder encL, Encoder encR)
{
	wheel_distance_left = encL.encoder_real_value*tickToMeter;
	wheel_distance_right = encR.encoder_real_value*tickToMeter;


	delta_l_left = wheel_distance_left - wheel_last_distance_left;
	delta_l_right = wheel_distance_right - wheel_last_distance_right;

	float delta_l_old = delta_l;
	float delta_alfa_old = delta_alfa;


	delta_l = (delta_l_left + delta_l_right) / 2;
	position.coordinates.X += ((delta_l + delta_l_old) / 2) * cos(position.alfa);
	position.coordinates.Y += ((delta_l + delta_l_old) / 2)* sin(position.alfa);


	delta_alfa = -(delta_l_left - delta_l_right) / d;
	position.alfa += ((delta_alfa + delta_alfa_old) / 2);


	wheel_last_distance_left = wheel_distance_left;
	wheel_last_distance_right = wheel_distance_right;

}




void Odometry::odometry_curved(Encoder encL, Encoder encR)
{

	wheel_distance_left = encL.encoder_real_value*tickToMeter;
	wheel_distance_right = encR.encoder_real_value*tickToMeter;

	delta_l_left = wheel_distance_left - wheel_last_distance_left;
	delta_l_right = wheel_distance_right - wheel_last_distance_right;
	delta_l = (delta_l_left + delta_l_right) / 2;

	delta_alfa = -(delta_l_left - delta_l_right) / d;
	if (abs(delta_l_right - delta_l_left) > arc_line_switch_treshold)
	{
		position.coordinates.X += (d*(delta_l_left + delta_l_right) / (delta_l_right - delta_l_left) / 2 * (sin(position.alfa + delta_alfa) - sin(position.alfa)));
		position.coordinates.Y -= (d*(delta_l_left + delta_l_right) / (delta_l_right - delta_l_left) / 2 * (cos(position.alfa + delta_alfa) - cos(position.alfa)));
	}
	else
	{
		position.coordinates.X += delta_l * cos(position.alfa);
		position.coordinates.Y += delta_l * sin(position.alfa);
	}
	position.alfa += delta_alfa;

	wheel_last_distance_left = wheel_distance_left;
	wheel_last_distance_right = wheel_distance_right;
}

