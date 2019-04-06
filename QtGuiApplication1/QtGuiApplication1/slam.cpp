
#include "slam.h"

RobotPosition Slam::locate(RobotPosition position_odometry, std::vector<LaserData>& scan)
{
	auto max_likehood = 0;
	estimate = position_odometry;
	Mapa map_scan(map_reference,false);

	for (int i=0;i<n_particles;i++)
	{

		std::normal_distribution<float> distribution_X(position_odometry.coordinates.X, dispersion_position);
		std::normal_distribution<float> distribution_Y(position_odometry.coordinates.Y, dispersion_position);
		std::normal_distribution <float> distribution_alfa(position_odometry.alfa, dispersion_angle);

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
		if (likehood>max_likehood)
		{
			max_likehood = likehood;
			estimate = particle;
		}
	}

	return estimate;;
}