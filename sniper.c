#include "common_robockey.h"
#ifdef SNIPER



void state_before_game(void);
void state_play(void);
void state_pause(void);

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
		case 0xA0:
			m_green(ON);
			m_red(ON);
			m_wait(1000);
			m_green(OFF);
			m_red(OFF);
			m_wait(1000);
			m_green(ON);
			m_red(ON);
			m_wait(1000);
			m_green(OFF);
			m_red(OFF);
			state_before_game();
			break;
		case 0xA1:
			state_play();
			break;
		default:
			state_before_game();
			break;
	}
}

void state_play()
{

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
}




void state_pause()
{
	set_left(0);
	set_right(0);
}

void state_detangle()
{
	set_left(-30);
	set_right(-30);
	while(!wireless_buffer_f);
	wireless_buffer_f = false;
	char inst = wireless_buffer[0];
	switch(inst)
	{
		case 0xA4:
			state_pause();
			return;
		case 0xA1:
			state_play();
			return;
		default:
			state_before_game();
			return;
	}	
}
#endif
