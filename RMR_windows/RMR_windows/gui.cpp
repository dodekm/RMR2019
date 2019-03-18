#include "pch.h"
#include "gui.h"

void GUI_Vlakno(void* param)
{

	//pre pracu/interface s robotom mas vytvorene vlakno, ktore 
	//bezi nezavisle a ma pristup k hlavnemu objektu robota -> je na dalsom riadku
	RobotControll* robot = (RobotControll*)param;
	

	while (1)
	{
		Mapa mapa = robot->getMap();


		//pre pristup k metodam robota pouzi sipkovu konvenciu 
		//priklad :
		//robot->build_map();
		//alebo
		//robot->set_command("stop");
		//RobotPosition poloha =robot->get_position()
	
		/*
		//Funkcie pre interface s robotom (popis k nim spravim neskor)

		void set_start(Point start);
		void set_target(Point target);
		RobotPosition get_position();
		RobotPosition get_wanted_position();
		robotSpeed get_motors_speed();
		Point get_target_point();
		Point get_starting_point();
		std::vector<RobotPosition>get_path();
		Mapa getMap();

		void set_command(std::string command);
		std::string get_command();
		void command_reset();
		//prikazy su v konkretne tieto:  "stop" "forward" "back" "left" "right" "auto" "save" "load" "reset" "clear" 

		//metody na interface s mapou este nie su hotove
	
		*/

	
	}
}
