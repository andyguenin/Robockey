#include "common_robockey.h"
#ifdef SNIPER

#define pi 3.14159265358979323

#define POSITIVE 1
#define NEGATIVE -1

#define dxp 57
#define dyp 10
#define dtp 4.7

#define dxpp 31
#define dypp 15
#define dtpp 3.14 

#define dxpn 58
#define dypn 30
#define dtpn 0

#define dxn -60
#define dyn -28
#define dtn 1.57

#define dxnp -66
#define dynp -48
#define dtnp 3.14 

#define dxnn -40
#define dynn -32
#define dtnn 0

#define debug

void indicate_r() {m_red(OFF); m_green(ON);}
void indicate_l() {m_red(ON); m_green(OFF);}
void indicate_n() {m_red(OFF); m_green(OFF);}
void indicate_f() {m_red(ON); m_green(ON);}

double fix_theta(double);


void event_comm_test(void);
void event_goal(char);

void state_before_game(void);
void state_play(void);
void state_pause(void);
void state_detangle(void);
void adc(void);
int rotate(double);
double rotate_to_point(double x_c, double y_c, double x_d, double y_d);

bool first = true;
int direction = POSITIVE;

int play_state = 1;
int find_then_play_state = 1;
int sub_play_state = 1;
bool boost = false;

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
	play_state = 1;
	set_position(1024/2, 768/2);
	first = true;
	//state_before_game();

	m_usb_init();
	while(!m_usb_isconnected());
	state_play();

}

void state_before_game(void)
{
	double x = 0;
	double y = 0;
	double theta = 0;

	unsigned int blobs[12];
	m_wii_read(blobs);
	get_position(blobs, &x, &y, &theta);
	
	theta = fix_theta(theta);

	if(theta < pi / 2 || theta > 3 * pi / 2)
	{
		direction = true;
	}
	else
	{
		direction = false;
	}

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
	unsigned int blobs[12];
	m_wii_read(blobs);
	double x = 0;
	double y = 0;
	double t;
	while(1)
	{
//		get_position(blobs, &x, &y, &t);

		t = fix_theta(t);

		if(first)
		{
			if(t < pi/2 || t > 3*pi/2)
			{
				direction = POSITIVE;
			}
			else
			{
				direction = NEGATIVE;
			}		
		}

	
		adc();
		m_usb_tx_string("\n\n\nleft");
		m_usb_tx_int(a_left);
		m_usb_tx_string("\ncenter");
		m_usb_tx_int(a_center);
		m_usb_tx_string("\nright");
		m_usb_tx_int(a_right);
		m_usb_tx_string("\npos");
		m_usb_tx_int(a_pos);
		m_usb_tx_string("\nside");
		m_usb_tx_int(a_side);
		m_wait(300);

		if(a_center > 300 || a_left > 300 || a_right > 300)
		{
			if(a_center >= a_left && a_center >= a_right)
			{
				if(a_pos > 990)
				{
					if(y > 6)
					{
						if(direction == POSITIVE)
						{
							indicate_r();
							m_usb_tx_string("\n\ntoo far left");
							set_left(40);
							set_right(35);
						}
						else
						{
							m_usb_tx_string("\n\ntoo far right");
							indicate_l();
							set_left(35);
							set_right(40);
						}
					}
					else
					{
						if(y < -6)
						{
							if(direction == POSITIVE)
							{
								indicate_l();
								m_usb_tx_string("\n\ntoo far right");
								set_left(35);
								set_right(40);
							}
							else
							{
								m_usb_tx_string("\n\ntoo far left");
								indicate_r();
								set_left(40);
								set_right(35);
							}
						}
						else
						{
							m_usb_tx_string("\n\ngo forward");
							indicate_f();
							set_left(60);
							set_right(60);
						}
					}

				}
				else
				{
					indicate_f();
					m_usb_tx_string("\n\nmove forward to puck");
					set_left(40);
					set_right(40);
				}
			}
			else
			{
				if(a_left >= a_center && a_left > a_right)
				{
					indicate_l();
					m_usb_tx_string("\n\nrotate left to puck");
					set_left(35);
					set_right(40);
				}
				else
				{
					if(a_right >= a_center && a_right > a_center)
					{
						indicate_r();
						m_usb_tx_string("\n\nrotate right to puck");
						set_left(40);
						set_right(35);
					}
					else
					{
						if(a_center > a_left && a_center > a_right)
						{
							indicate_f();
							m_usb_tx_string("\n\nmove forward to puck");
							set_left(40);
							set_right(40);
						}
						else
						{
							indicate_n();
							m_usb_tx_string("\n\nno puck found");
							set_left(15);
							set_right(-15);
						}
					}
				}
			}
		}
		else
		{
			set_left(15);
			set_right(-15);
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
				case PAUSE:
					state_pause();
					return;
				case PLAY:
					1 == 1;
					return;
				case HALFTIME:
					direction = !direction;
					state_pause();
					break;
				case COMMTEST:
					event_comm_test();
					break;
				default:
					state_pause();
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

double rotate_to_point(double x_c, double y_c, double x_d, double y_d)
{
	return atan2(y_d - y_c, x_d - x_c);

}

int rotate(double theta)
{
	if(theta < pi / 2 || theta > 3*pi/2 )
	{
		if(theta < -pi/2)
		{
			set_left(20);
			set_right(-20);
			return 1;
		}
		if(theta < -pi / 4)
		{
			set_left(20);
			set_right(-20);
			return 2;
		}
		if(theta < -pi / 8)
		{
			set_left(15);
			set_right(-15);
			return 3;
		}
	}
	else
	{
		if(theta > pi/2)
		{
			set_left(-20);
			set_right(20);
			return 1;
		}
		if(theta > pi / 4)
		{
			set_left(-20);
			set_right(20);
			return 2;
		}
		if(theta > pi / 8)
		{
			set_left(-15);
			set_right(15);
			return 3;
		}

	}
	return 0;
}

#endif
