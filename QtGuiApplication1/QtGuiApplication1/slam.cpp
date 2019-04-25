
#include "slam.h"

RobotPosition Slam::locate(RobotPosition position_odometry, std::vector<LaserData>& scan)
{
	long max_likehood = 0;
	long best_scan_sum = 0;
	RobotPosition estimate_last = estimate;

	RobotPosition position_center = weighted_position(estimate, position_odometry, feedback_gain, odometry_gain);
	
	Mapa map_scan(map_reference,false);

	for (int p=0;p<n_particles;p++)
	{

		std::normal_distribution<float> distribution_X(position_center.coordinates.X, dispersion_position);
		std::normal_distribution<float> distribution_Y(position_center.coordinates.Y, dispersion_position);
		std::normal_distribution <float> distribution_alfa(position_center.alfa, dispersion_angle);

		RobotPosition particle = RobotPosition(distribution_X(generator),distribution_Y(generator),distribution_alfa(generator));
		map_scan.clearMap();

		for (int i = 0; i < scan.size(); i++)
		{
				map_scan.addPoint(lidar_measure_2_point(scan[i], particle), cell_obstacle);	
		}

		Mapa AND(map_reference && map_scan);
		long likehood = AND.sum_elements();
		

		if (likehood>max_likehood)
		{
			max_likehood = likehood;
			estimate = particle;
			best_scan_sum=map_scan.sum_elements();
		}
	}

	float new_quality = (float)max_likehood / (float)best_scan_sum;
	if (new_quality > estimate_quality)
	{
		estimate_quality = new_quality;
	}
	else
	{
		estimate = estimate_last;
	}

	return estimate;
}


RobotPosition weighted_position(RobotPosition A, RobotPosition B, float weight_A, float weight_B)
{
	return(A*weight_A + B * weight_B) / (weight_A + weight_B);
}
