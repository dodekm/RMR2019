
#include "slam.h"

RobotPosition Slam::locate(RobotPosition position_odometry, std::vector<LaserData>& scan)
{
	likehood_vector.clear();
	auto max_likehood = 0;
	
	RobotPosition estimate_last = estimate;

	RobotPosition position_center = weighted_position(estimate, position_odometry, feedback_gain, odometry_gain);
	
	Mapa map_scan(map_reference,false);

	for (int i=0;i<n_particles;i++)
	{

		std::normal_distribution<float> distribution_X(position_center.coordinates.X, dispersion_position);
		std::normal_distribution<float> distribution_Y(position_center.coordinates.Y, dispersion_position);
		std::normal_distribution <float> distribution_alfa(position_center.alfa, dispersion_angle);

		RobotPosition particle = RobotPosition(distribution_X(generator),distribution_Y(generator),distribution_alfa(generator));
		map_scan.clearMap();

		for (int i = 0; i < scan.size(); i++)
		{
			if (lidar_check_measure(scan[i]))
			{
				map_scan.addPoint(lidar_measure_2_point(scan[i], particle), cell_obstacle);
			}

		}
		Mapa AND = map_reference && map_scan;
		auto likehood = AND.sum_elements();
		
		likehood_vector.push_back(likehood);

		if (likehood>max_likehood)
		{
			max_likehood = likehood;
			estimate = particle;
		}
	}

	estimate_quality = (float)max_likehood / map_scan.sum_elements();



	return estimate;
}


RobotPosition weighted_position(RobotPosition A, RobotPosition B, float weight_A, float weight_B)
{
	return(A*weight_A + B * weight_B) / (weight_A + weight_B);
}
