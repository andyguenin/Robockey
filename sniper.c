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
void adc(void);
bool first = true;
bool direction = false;

bool g_pos = false;
bool s_pos = false;

int a_left = 0;
int a_center = 0;
int a_right = 0;
int a_pos = 0;
int a_side = 0;

void sniper()
{

	init_all();	
	// set voltage reference to 5V
	 clear(ADMUX, REFS1);
	  set(ADMUX, REFS0);

	  m_disableJTAG();//allowing gpio of F pins

	  //setting ADC prescaler
	  set(ADCSRA,ADPS2);
	  set(ADCSRA,ADPS1);
	  set(ADCSRA,ADPS0);

	  //setting pins to turn off digital circuitry
	  set(DIDR0,ADC1D);//setting F1
	  set(DIDR0,ADC4D);//setting F4
	  set(DIDR0,ADC5D);//setting F5
	  set(DIDR0,ADC6D);//setting F6
	  set(DIDR0,ADC7D);//setting F7

	set(ADCSRA, ADEN);

		while(1)
		{
			adc();
			if(a_pos >= 903)
			{
				s_pos = true;
			}
			else
			{
				s_pos = false;
			}

	
			if(a_left < 500 && a_right < 500)
			{
				set_left(20);
				set_right(-20);
			}
			if(a_left > 900 && a_left > a_center)
			{
				set_right(20);
				set_left(-20);
			}
			if(a_right > 900 && a_right > a_center)
			{
				set_right(-20);
				set_left(20);
			}
			if(a_center> a_left && a_center > a_right && a_center > 800 && !s_pos)
			{
				set_right(20);
				set_left(20);
			}

			if(s_pos)
			{	
				m_green(OFF);
				m_red(ON);
				set_right(0);
				set_left(0);
			}
			else
			{
		
				m_green(ON);
				m_red(OFF);
			}
			
		}
	//state_before_game();

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
		adc();
		if(a_pos >= 1003)
		{
			s_pos = true;
		}
		else
		{
			s_pos = false;
		}
				
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
				case 0x01:
					g_pos = true;
					break;
				case 0x02:
					g_pos = false;
					break;					
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

void adc()
{
	//F1
	clear(ADCSRB, MUX5);
	clear(ADMUX, MUX2);
	clear(ADMUX, MUX1);
	set(ADMUX, MUX0);

	set(ADCSRA, ADSC);

	while(!check(ADCSRA, ADIF));
	a_left = ADC;
	set(ADCSRA, ADIF);



	//F4
	clear(ADCSRB, MUX5);
	set(ADMUX, MUX2);
	clear(ADMUX, MUX1);
	clear(ADMUX, MUX0);

	set(ADCSRA, ADSC);

	while(!check(ADCSRA, ADIF));
	a_center = ADC;
	set(ADCSRA, ADIF);


	//F5
	clear(ADCSRB, MUX5);
	set(ADMUX, MUX2);
	clear(ADMUX, MUX1);
	set(ADMUX, MUX0);

	set(ADCSRA, ADSC);

	while(!check(ADCSRA, ADIF));
	a_right = ADC;
	set(ADCSRA, ADIF);


	//F6
	clear(ADCSRB, MUX5);
	set(ADMUX, MUX2);
	set(ADMUX, MUX1);
	clear(ADMUX, MUX0);

	set(ADCSRA, ADSC);

	while(!check(ADCSRA, ADIF));
	a_pos = ADC;
	set(ADCSRA, ADIF);


	//F7
	clear(ADCSRB, MUX5);
	set(ADMUX, MUX2);
	set(ADMUX, MUX1);
	set(ADMUX, MUX0);

	set(ADCSRA, ADSC);

	while(!check(ADCSRA, ADIF));
	a_side = ADC;
	set(ADCSRA, ADIF);

}
void event_goal(char side)
{

}
#endif
