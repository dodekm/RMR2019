
#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
	
	QApplication app(argc, argv);
	QtGuiApplication1 gui;
	
	RobotControll robot;
	
	QObject::connect(&gui, SIGNAL(command_change_sig(std::string)), &robot, SLOT(set_command(std::string)));
	QObject::connect(&gui, SIGNAL(start_threads_sig(void)), &robot, SLOT(start_threads(void)));

	QObject::connect(&robot, SIGNAL(map_update_sig(Mapa)), &gui, SLOT(map_update(Mapa)));
	QObject::connect(&robot, SIGNAL(odometry_update_sig(RobotPosition)), &gui, SLOT(odometry_update(RobotPosition)));


   
	gui.show();
	return app.exec();
	robot.robotthreadHandle.std::thread::join();
	robot.laserthreadHandle.std::thread::join();
}

