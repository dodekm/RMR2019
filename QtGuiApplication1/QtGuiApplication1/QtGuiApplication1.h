#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"

#include "RMR_windows.h"

void map_render(QPainter& paint, QPen& pen, Mapa& map, QRect rect);

class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication1(QWidget *parent = Q_NULLPTR);

	Ui::QtGuiApplication1Class* get_ui_ptr()
	{
		return &ui;
	}

	void paintEvent(QPaintEvent *e);

public slots:

   

    

	void map_update(Mapa);
	void scope_update(Mapa);
	void odometry_update(Robot_feedback);


signals:
	void command_change_sig(robot_command command);
	void set_threads_enabled_sig(bool status);
	void set_maping_enabled_sig(bool status);
	void set_map_with_path_enabled_sig(bool status);
	void set_slam_enabled_sig(bool status);

	void start_threads_sig(void);
	void stop_threads_sig();
	void set_filename_sig(std::string);
	void setip_sig(std::string);
	void set_target_sig(Point);
	void addPointToPath_sig(RobotPosition);


private slots:
    void on_pushButton_clear_path_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_locate_clicked();

    void on_checkBox_2_stateChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

	void on_startButton_clicked();

	void on_pushButton_forward_clicked();

	void on_pushButton_back_clicked();

	void on_pushButton_right_clicked();

	void on_pushButton_left_clicked();

	void on_pushButton_forward_pressed();

	void on_pushButton_forward_released();

	void on_pushButton_right_pressed();

	void on_pushButton_right_released();

	void on_pushButton_back_pressed();

	void on_pushButton_back_released();

	void on_pushButton_left_pressed();

	void on_pushButton_left_released();

	void on_pushButton_stop_clicked();

	void on_pushButton_find_path_clicked();

	void on_pushButton_reset_clicked();

	void on_pushButton_auto_clicked();

	void on_pushButton_clear_map_clicked();

	void on_pushButton_load_map_clicked();

	void on_pushButton_save_map_clicked();

	void on_pushButton_go_clicked();

    void on_checkBox_3_stateChanged(int arg1);

private:
	Ui::QtGuiApplication1Class ui;
	Mapa map;
	Mapa scope;
	bool connection_status = false;
	
};



