#pragma once

#include "map.h"
#include <random>
#include <misc.h>

#define histogram_treshold_slam 0.002

class Slam
{
	
public:
	RobotPosition locate(RobotPosition, std::vector<LaserData>& scan);
	float dispersion_position;
	float dispersion_angle;
	unsigned int n_particles;

	float feedback_gain;
	float odometry_gain;

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


RobotPosition weighted_position(RobotPosition A, RobotPosition B, float weight_A, float weight_B);

