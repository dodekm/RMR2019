
#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	
	QApplication app(argc, argv);
	QtGuiApplication1 gui;
	
	RobotControll robot;
	robot.gui_ptr = &gui;

	qRegisterMetaType<Mapa>();
	qRegisterMetaType<Robot_feedback>();

	

	QObject::connect(&gui, SIGNAL(command_change_sig(robot_command)), &robot, SLOT(push_command(robot_command)));
	QObject::connect(&gui, SIGNAL(start_threads_sig(void)), &robot, SLOT(start_threads(void)));
	QObject::connect(&gui, SIGNAL(stop_threads_sig(void)), &robot, SLOT(stop_threads(void)));

	QObject::connect(&gui, SIGNAL(set_threads_enabled_sig(bool)), &robot, SLOT(set_threads_enabled(bool)));
	QObject::connect(&gui, SIGNAL(set_maping_enabled_sig(bool)), &robot, SLOT(set_maping_enabled(bool)));
	QObject::connect(&gui, SIGNAL(set_slam_enabled_sig(bool)), &robot, SLOT(set_slam_enabled(bool)));
	QObject::connect(&gui, SIGNAL(set_map_with_path_enabled_sig(bool)), &robot, SLOT(set_map_with_path_enabled(bool)));

	QObject::connect(&gui, SIGNAL(set_filename_sig(std::string)), &robot, SLOT(setfilename(std::string)));
	QObject::connect(&gui, SIGNAL(setip_sig(std::string)), &robot, SLOT(setip(std::string)));
	QObject::connect(&gui, SIGNAL(set_target_sig(Point)), &robot, SLOT(set_target(Point)));
	QObject::connect(&gui, SIGNAL(addPointToPath_sig(RobotPosition)), &robot, SLOT(addPointToPath(RobotPosition)));

	QObject::connect(&robot, SIGNAL(map_update_sig(Mapa)), &gui, SLOT(map_update(Mapa)));
	QObject::connect(&robot, SIGNAL(scope_update_sig(Mapa)), &gui, SLOT(scope_update(Mapa)));
	QObject::connect(&robot, SIGNAL(odometry_update_sig(Robot_feedback)), &gui, SLOT(odometry_update(Robot_feedback)));



	gui.show();
	gui.get_ui_ptr()->doubleSpinBox->setValue(target_X);
	gui.get_ui_ptr()->doubleSpinBox_2->setValue(target_Y);
	return app.exec();
	robot.stop_threads();
}

