#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"

#include "RMR_windows.h"

class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication1(QWidget *parent = Q_NULLPTR);

	Ui::QtGuiApplication1Class* get_ui_ptr()
	{
		return &ui;
	}

public slots:

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

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

	void map_update(Mapa);
	void odometry_update(RobotPosition);


signals:
	void command_change_sig(std::string command);
	void start_threads_sig(void);
	void target_point_set_sig(int, int);
	void push_point_to_path_sig(void);

private:
	Ui::QtGuiApplication1Class ui;
	Mapa map;
	
};


class RobotControll_QT :public QObject, public RobotControll
{
	Q_OBJECT

public:
	Ui::QtGuiApplication1Class* ui;
private:

public slots:

	void command_change(std::string command);
	void target_point_set(int X,int Y);
	void robot_start_threads();
	void push_point_to_path(void);

signals:

	void map_update_sig(Mapa);
	void odometry_update_sig(RobotPosition);


};



