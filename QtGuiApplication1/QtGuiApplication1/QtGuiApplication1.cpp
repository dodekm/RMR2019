#include "QtGuiApplication1.h"

#include <QPainter>


QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent) 
{
	ui.setupUi(this);
	
}

void QtGuiApplication1::on_startButton_clicked()
{



	if (connection_status == false)
	{
		emit setip_sig(ui.lineEdit->text().toStdString());
		emit start_threads_sig();
		ui.startButton->setText("Unconnect");
	}
	else
	{
	
		emit stop_threads_sig();
		ui.startButton->setText("Connect");

	}
	connection_status = !connection_status;

	
	
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
	emit command_change_sig(robot_command::forward);
}

void QtGuiApplication1::on_pushButton_forward_released()
{
	emit command_change_sig(robot_command::stop);
}

void QtGuiApplication1::on_pushButton_right_pressed()
{
	emit command_change_sig(robot_command::right);
}

void QtGuiApplication1::on_pushButton_right_released()
{
	emit command_change_sig(robot_command::stop);
}

void QtGuiApplication1::on_pushButton_back_pressed()
{
	emit command_change_sig(robot_command::back);
}

void QtGuiApplication1::on_pushButton_back_released()
{
	emit command_change_sig(robot_command::stop);
}

void QtGuiApplication1::on_pushButton_left_pressed()
{
	emit command_change_sig(robot_command::left);
}

void QtGuiApplication1::on_pushButton_left_released()
{
	emit command_change_sig(robot_command::stop);
}


void QtGuiApplication1::on_pushButton_stop_clicked()
{
	emit command_change_sig(robot_command::stop);
}

void QtGuiApplication1::on_pushButton_find_path_clicked()
{
	emit command_change_sig(robot_command::find);
}

void QtGuiApplication1::on_pushButton_reset_clicked()
{
	emit command_change_sig(robot_command::reset);
}

void QtGuiApplication1::on_pushButton_auto_clicked()
{
	emit command_change_sig(robot_command::automatic);
}

void QtGuiApplication1::on_pushButton_clear_path_clicked()
{
	emit command_change_sig(robot_command::clear_path);
}

void QtGuiApplication1::on_pushButton_clear_map_clicked()
{
	emit command_change_sig(robot_command::clear);
}

void QtGuiApplication1::on_pushButton_load_map_clicked()
{
	emit set_filename_sig(ui.lineEdit_2->text().toStdString());
	emit command_change_sig(robot_command::load);
}

void QtGuiApplication1::on_pushButton_save_map_clicked()
{
	emit set_filename_sig(ui.lineEdit_2->text().toStdString());
	emit command_change_sig(robot_command::save);
}

void QtGuiApplication1::on_pushButton_go_clicked()
{
	emit addPointToPath_sig(RobotPosition((float)ui.spinBox->value(),(float)ui.spinBox_2->value()));
}

void QtGuiApplication1::on_spinBox_valueChanged(int arg1)
{
	emit set_target_sig(Point{ (float)ui.spinBox->value(),(float)ui.spinBox_2->value() });
}

void QtGuiApplication1::on_spinBox_2_valueChanged(int arg1)
{
	emit set_target_sig(Point{ (float)ui.spinBox->value(),(float)ui.spinBox_2->value() });
}

void QtGuiApplication1::paintEvent(QPaintEvent *e)
{
	QRect rect = ui.frame->geometry();
	QPainter paint(this);
	QPen pen(Qt::black, 4, Qt::SolidLine);
	paint.setPen(pen);
	paint.drawRect(rect);
	
	

	Matrix_position i;
	
	for (i.Y = 0; i.Y < map.get_rows(); i.Y++)
	{
		for (i.X = 0; i.X < map.get_cols(); i.X++)
		{
			
			if (map[i] == cell_obstacle)
				pen.setColor(Qt::magenta);
			else if (map[i] == cell_path)
				pen.setColor(Qt::blue);
			else if (map[i] == cell_breakpoint)
				pen.setColor(Qt::cyan);
			else if (map[i] == cell_finish)
				pen.setColor(Qt::red);
			else if (map[i] == cell_start)
				pen.setColor(Qt::green);
			else if (map[i] == cell_robot)
				pen.setColor(Qt::black);
			else 
				continue;
		
			paint.setPen(pen);
			paint.drawPoint(rect.topLeft().x() + rect.width()*i.X / map.get_cols(), rect.topLeft().y() + rect.height()*i.Y / map.get_rows());
			
			//paint.drawPoint(i.X * 3 + rect.topLeft().x(), i.Y * 3 + rect.topLeft().y());
			//paint.drawRect(QRect(i.X * 3 + rect.topLeft().x(), i.Y * 3 + rect.topLeft().y(), 1, 1));
			

		}
	}
	
	
}


void QtGuiApplication1::map_update(Mapa mapa)
{
	
	this->map = mapa;
	update();
}


void QtGuiApplication1::odometry_update(Robot_feedback data)
{
	ui.lcdNumber->display(data.actual_position.coordinates.X);
	ui.lcdNumber_2->display(data.actual_position.coordinates.Y);
	ui.lcdNumber_3->display(data.actual_position.alfa);
	ui.lcdNumber_4->display(data.motors_speed.translation_speed);
	ui.lcdNumber_5->display(data.motors_speed.radius);

	ui.lcdNumber_6->display(data.wanted_position.coordinates.X);
	ui.lcdNumber_7->display(data.wanted_position.coordinates.Y);

	ui.lcdNumber_8->display(data.target.X);
	ui.lcdNumber_9->display(data.target.Y);

	ui.label_9->setText(QString(QString::fromStdString(data.command_string)));

	if (data.connection_status == true)
		ui.label_12->setText("connected");
	else 
		ui.label_12->setText("unconnected");
	

}

