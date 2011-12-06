#include "common_robockey.h"

void move_to(double x_current, double y_current, double t_current, int speed, double x_des, double y_des, double t_des)
{
	double t_diff = t_current;
	if(t_diff >= pi)
	{
		t_current -= 2 * pi;
		m_red(ON);
		m_green(OFF);
	}
	else
	{
		m_red(OFF);
		m_green(ON);
	}
	set_right(speed +  ((int)(5 * t_diff)));
	set_left(speed -  ((int)(5 * t_diff)));

}
