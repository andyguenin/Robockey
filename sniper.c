#include "common_robockey.h"
#ifdef SNIPER

char input[PACKET_SIZE];

void state_before_game(void);
void state_play(void);
void state_pause();

void sniper()
{

	init_all();	
	state_before_game();

}

void state_before_game(void)
{

	while(!wireless_buffer_full());

	get_wireless_buffer(input);
	
	char inst = input[0];
	
	switch(inst) 
	{
		case 0xA0:
			m_green(ON);
			m_red(ON);
			wait(1);
			m_green(OFF);
			m_red(OFF);
			wait(1);
			m_green(ON);
			m_red(ON);
			wait(1);
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

	while(!wireless_buffer_full());

	get_wireless_buffer(input);
	
	char inst = input[0];
	
	switch(inst) 
	{
		case 0xA4:
			state_pause();
			break;
		case 0xA1:
			state_play();
			break;
		default:
			state_before_game();
			break;
	}
}




void state_pause()
{
	set_left(0);
	set_right(0);

	while(!wireless_buffer_full());
}
#endif
