
#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
	
	QApplication app(argc, argv);
	QtGuiApplication1 gui;
	
	RobotControll_QT robot;

	QObject::connect(&gui, SIGNAL(command_change_sig(std::string)), &robot, SLOT(command_change(std::string)));
	QObject::connect(&gui, SIGNAL(start_threads_sig(void)), &robot, SLOT(robot_start_threads(void)));
	QObject::connect(&gui, SIGNAL(target_point_set_sig(int, int)), &robot, SLOT(target_point_set(int, int)));
	QObject::connect(&gui, SIGNAL(push_point_to_path_sig(void)), &robot, SLOT(push_point_to_path(void)));

	QObject::connect(&robot, SIGNAL(map_update_sig(Mapa)), &gui, SLOT(map_update(Mapa)));
	QObject::connect(&robot, SIGNAL(odometry_update_sig(RobotPosition)), &gui, SLOT(odometry_update(RobotPosition)));

	robot.ui = gui.get_ui_ptr();
   
	gui.show();
	return app.exec();
	robot.robotthreadHandle.std::thread::join();
	robot.laserthreadHandle.std::thread::join();
}

