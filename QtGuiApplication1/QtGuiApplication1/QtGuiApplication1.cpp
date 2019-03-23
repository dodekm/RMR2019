#include "QtGuiApplication1.h"


QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent) 
{
	ui.setupUi(this);
	
}

void QtGuiApplication1::on_startButton_clicked()
{
	emit start_threads_sig();
}

void QtGuiApplication1::on_pushButton_forward_clicked()
{
	
}

void QtGuiApplication1::on_pushButton_back_clicked()
{
	
	
}

void QtGuiApplication1::on_pushButton_right_clicked()
{
	
}

void QtGuiApplication1::on_pushButton_left_clicked()
{
	
}

void QtGuiApplication1::on_pushButton_forward_pressed()
{
	emit command_change_sig("forward");
}

void QtGuiApplication1::on_pushButton_forward_released()
{
	emit command_change_sig("stop");
}

void QtGuiApplication1::on_pushButton_right_pressed()
{
	emit command_change_sig("right");
}

void QtGuiApplication1::on_pushButton_right_released()
{
	emit command_change_sig("stop");
}

void QtGuiApplication1::on_pushButton_back_pressed()
{
	emit command_change_sig("back");
}

void QtGuiApplication1::on_pushButton_back_released()
{
	emit command_change_sig("stop");
}

void QtGuiApplication1::on_pushButton_left_pressed()
{
	emit command_change_sig("left");
}

void QtGuiApplication1::on_pushButton_left_released()
{
	emit command_change_sig("stop");
}


void QtGuiApplication1::on_pushButton_stop_clicked()
{
	emit command_change_sig("stop");
}

void QtGuiApplication1::on_pushButton_find_path_clicked()
{
	emit command_change_sig("find");
}

void QtGuiApplication1::on_pushButton_reset_clicked()
{
	emit command_change_sig("reset");
}

void QtGuiApplication1::on_pushButton_auto_clicked()
{
	emit command_change_sig("auto");
}

void QtGuiApplication1::on_pushButton_clear_map_clicked()
{
	emit command_change_sig("clear");
}

void QtGuiApplication1::on_pushButton_load_map_clicked()
{
	emit command_change_sig("load");
}

void QtGuiApplication1::on_pushButton_save_map_clicked()
{
	emit command_change_sig("save");
}

void QtGuiApplication1::on_pushButton_go_clicked()
{
	//emit push_point_to_path_sig();
}

void QtGuiApplication1::on_spinBox_valueChanged(int arg1)
{
	//emit target_point_set_sig(ui.spinBox->value(), ui.spinBox_2->value());
}

void QtGuiApplication1::on_spinBox_2_valueChanged(int arg1)
{
	//emit target_point_set_sig(ui.spinBox->value(), ui.spinBox_2->value());
}



void QtGuiApplication1::map_update(Mapa)
{
	
	

}

void QtGuiApplication1::odometry_update(RobotPosition pos)
{
	ui.lcdNumber->display( pos.coordinates.X);
	ui.lcdNumber_2->display(pos.coordinates.Y);
	ui.lcdNumber_3->display(pos.alfa);
}
/*

void RobotControll_QT::command_change(std::string command)
{
	if (command == "load" || command == "save")
		RobotControll::setfilename(ui->lineEdit_2->text().toStdString());
	RobotControll::set_command(command);
}

void RobotControll_QT::target_point_set(int X, int Y)
{
	RobotControll::set_target(Point{ (float)X, (float)Y });
}

void RobotControll_QT::push_point_to_path(void)
{
	RobotControll::addPointToPath(RobotControll::get_target_point());
}

void RobotControll_QT::robot_start_threads(void)
{
	setip(ui->lineEdit->text().toStdString());
	parent_robot_update_callback = &::robot_update_callback;
	parent_map_update_callback = &::map_update_callback;
	parent_ptr = (void*)this;
	RobotControll::start_threads();
	//emit odometry_update_sig(get_position());
	//emit map_update_sig(getMap());
}


*/