#include "common_robockey.h"
#ifdef SNIPER

#define pi 3.14159265358979323


double fix_theta(double);


void event_comm_test(void);
void event_goal(char);

void state_before_game(void);
void state_play(void);
void state_pause(void);
void state_detangle(void);
bool first = true;
bool direction = false;

void sniper()
{

	init_all();	
	state_before_game();

}

void state_before_game(void)
{
	while(1)
	{
		m_red(ON);
		while(!wireless_buffer_f);
		m_red(OFF);
		wireless_buffer_f = false;
		switch(wireless_buffer[0]) 
		{
			case COMMTEST:
				event_comm_test();
				break;
			case PLAY:
				state_play();
				return;
			default:
				state_before_game();
				break;
		}
	}
}

void event_comm_test()
{
	for(int i = 0; i < 5; i++)
	{
		m_green(ON);
		m_red(ON);
		m_wait(100);
		m_green(OFF);
		m_red(OFF);
		m_wait(100);
	}

}
	
double fix_theta(double t)
{
	double theta = t;
	while(theta < 0)
		theta += 2 * pi;
	while(theta >= 2 * pi)
		theta -= 2 * pi;
	return theta;
}
		
void state_play()
{
	m_green(ON);
	while(1)
	{
		double x = 0;
		double y = 0;
		double theta = 0;

		unsigned int blobs[12];
		m_wii_read(blobs);
		get_position(blobs, &x, &y, &theta);
		
		theta = fix_theta(theta);

		if(first)
		{
			if(theta < pi / 2 || theta > 3 * pi / 2)
			{
				m_green(ON);
				m_red(OFF);
			}
			else
			{
				m_red(ON);
				m_green(OFF);
			}

			first = false;
		}

		if(wireless_buffer_f)
		{
			wireless_buffer_f = false;
			switch(wireless_buffer[0])
			{
				case 0xA4:
					state_pause();
					break;
				case 0xA1:
					state_play();
					return;
				default:
					state_before_game();
					return;
			}
		}
	}

/*
	m_green(ON);
	set_right(30);
	set_left(-30);
	for(int i = 0; i < 30 && !wireless_buffer_f; i++)
	{
		m_wait(100);
	}

	set_right(-20);
	set_left(20);
	for(int i = 0; i < 30 && !wireless_buffer_f; i++)
	{
		m_wait(100);
	}

	
	wireless_buffer_f = false;
	char inst = wireless_buffer[0];
	
	switch(inst) 
	{
		case 0xA4:
			state_pause();
			break;
		case 0xA1:
			state_play();
			return;
		default:
			state_before_game();
			return;
	}
	*/
	
}




void state_pause()
{
	while(1)
	{
		set_left(0);
		set_right(0);
		while(!wireless_buffer_f);
		wireless_buffer_f = false;
		switch(wireless_buffer[0])
		{
			case PLAY:
				state_play();
				break;
			case DETANGLE:
				state_detangle();
				break;
			case GOALA:
				event_goal('a');
				break;
			case GOALB:
				event_goal('b');
				break;
		}
	}
}

void state_detangle()
{
	while(1)
	{
		set_left(-30);
		set_right(-30);
		while(!wireless_buffer_f);
		wireless_buffer_f = false;
		char inst = wireless_buffer[0];
		switch(inst)
		{
			case PAUSE:
				state_pause();
				return;
			case PLAY:
				state_play();
				return;
			default:
				break;
		}	
	}
}

void event_goal(char side)
{

}
#endif
