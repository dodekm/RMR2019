#pragma once

#include "map.h"
#include <random>
#include <misc.h>
class Slam
{
	
public:
	RobotPosition locate(RobotPosition, std::vector<LaserData>& scan);
	float dispersion_position=0.2;
	float dispersion_angle=0.5;
	RobotPosition estimate;
	Mapa map_reference;

	Slam()
	{

	}

	Slam(Mapa map_reference):map_reference(map_reference)
	{
	
	}

	~Slam()
	{


	}

private:
	
	unsigned int n_particles=100;
	std::default_random_engine generator;

};
