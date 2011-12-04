#include "common_robockey.h"
#ifdef SNIPER






void event_comm_test(void);
void event_goal(char);

void state_before_game(void);
void state_play(void);
void state_pause(void);
void state_detangle(void);


void sniper()
{

	init_all();
	state_before_game();

}

void state_before_game(void)
{

	m_red(ON);
	while(!wireless_buffer_f);
	m_red(OFF);
	wireless_buffer_f = false;
	char inst = wireless_buffer[0];
	switch(wireless_buffer[0]) 
	{
		case COMMTEST:
			state_comm_test();
			state_before_game();
			break;
		case PLAY:
			state_play();
			break;
		default:
			state_before_game();
			break;
	}
}

void event_comm_test()
{
	for(int i = 0; i < 10; i++)
	{
		m_green(ON);
		m_red(ON);
		m_wait(100);
		m_green(OFF);
		m_red(OFF);
		m_wait(100);
	}
}
			
void state_play()
{
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
				return;
			case DETANGLE:
				state_detangle();
				return;
			case GOALA:
				
			case GOALB:
				
			case default:
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
