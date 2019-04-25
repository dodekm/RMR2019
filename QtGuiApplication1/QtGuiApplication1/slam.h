#pragma once

#include "map.h"
#include <random>
#include <misc.h>


RobotPosition weighted_position(RobotPosition A, RobotPosition B, float weight_A, float weight_B);


class Slam
{
	
public:
	RobotPosition locate(RobotPosition, std::vector<LaserData>& scan);
	float dispersion_position=0.1;
	float dispersion_angle=0.2;
	unsigned int n_particles = 100;

	float feedback_gain = 0.5;
	float odometry_gain = 0.5;

	float quality_treshold = 0.5;
	float estimate_quality=0.0;
	
	RobotPosition estimate;
	Mapa map_reference;
	std::vector <float>likehood_vector;
	
	Slam()
	{
	
	}

	Slam(Mapa map_reference):map_reference(map_reference)
	{
	
	}

	~Slam()
	{


	}


	float get_dispersion()
	{
		return vector_standard_deviation(likehood_vector);
	}

private:
	
	std::default_random_engine generator;

};




