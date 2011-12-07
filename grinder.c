                                                                     
                                                                     
                                                                     
                                             
#include "common_robockey.h"
#ifdef GRINDER

unsigned int blobs[12];

double x;
double y;
double theta;

double x_goal;
double y_goal;
double theta_goal;

double fix_theta(double);

char buffer[12];

unsigned int i = 0;

bool j = 0;

bool second_half = false;
bool align = 0;
bool sit_1 = false;
bool sit_2 = false;
bool start = true;
bool possession = false;
bool target = false;

void grinder(void)
{

	init_all();

	//set voltage reference to 5V
	clear(ADMUX, REFS1);
	set(ADMUX, REFS0);

	//set ADC pre-scaler
	set(ADCSRA,ADPS2);
	set(ADCSRA,ADPS1);
	set(ADCSRA,ADPS0);

	//set pin to turn off digital circuitry
	set(DIDR0,ADC6D);
	set(DIDR0,ADC7D);
	set(DIDR2,ADC10D);

	//set B0 for puck possession input
	clear(DDRB,0);

	//initialize position
	m_red(ON); m_green(ON); m_wait(300);
	m_red(OFF); m_green(OFF); m_wait(300);
	m_red(ON); m_green(ON); m_wait(300);
	m_wii_read(blobs);
	set_position(512,384);
	get_position(blobs, &x, &y, &theta);
	
	x = x_goal;
	y = y_goal;
	theta_goal = fix_theta(theta);	

	m_green(OFF); m_red(OFF); m_wait(300);
	m_red(ON); m_green(ON); m_wait(300);
	m_red(OFF); m_green(OFF); m_wait(300);

	char inst = wireless_buffer[0];

	while(1)
	{

		if(wireless_buffer_f)
		{
			wireless_buffer_f = false;
			inst = wireless_buffer[0];
		}
		// inst = 0xA1;

		switch(inst)
		{
			case 1:
			set_left(0);
			set_right(0);
			break;

			case 2:
			inst = 0xA1;
			break;

			case 0xA0: //command test
			m_green(ON);
			m_red(ON);
			m_wait(300);
			m_green(OFF);
			m_red(OFF);
			m_wait(300);
			m_green(ON);
			m_red(ON);
			m_wait(300);
			m_green(OFF);
			m_red(OFF);
			break;

			case 0xA1: //play

			//set channel selection to D7 for puck location
			clear(ADMUX,MUX0);
			set(ADMUX,MUX1);
			clear(ADMUX,MUX2);
			set(ADCSRB,MUX5);

			set(ADCSRA,ADEN);
			set(ADCSRA,ADSC);

			while(!check(ADCSRA,ADIF));
			while(start && ADC >= 975 && !wireless_buffer_f)
			{
				wireless_buffer_f = false; 

				set(ADCSRA,ADIF); 
				set(ADCSRA,ADSC); 

				set_left(100); 
				set_right(100);
			}

			start = false;

			m_wii_read(blobs);
			get_position(blobs, &x, &y, &theta);
			theta = fix_theta(theta);

			if(ADC < 985)
			{
				m_green(ON);
				
				if(i < 8000 && !j){set_left(10); set_right(30); i += 1;}
				if(i == 10000 && !j){i = 0; j = 1;}
				if(j){set_left(-50); set_right(-50); m_wait(500); i = 0; j = 0;}

				 target = false;
			}

			if(ADC >= 985){m_green(OFF); set_left(30); set_right(30); i = 0; j = 0; target = true; }

			set(ADCSRA,ADIF);
			clear(ADCSRA,ADEN);

			if(target && !wireless_buffer_f)
			{
				wireless_buffer_f = false;
			
				//set channel selection to F6 for puck possession
				clear(ADMUX,MUX0);
				set(ADMUX,MUX1);
				set(ADMUX,MUX2);
				clear(ADCSRB,MUX5);
					
				set(ADCSRA,ADEN);
				set(ADCSRA,ADSC);
				
				while(!check(ADCSRA,ADIF));
					
				if(ADC < 960){possession = false;}
				if(ADC >= 960)
				{
					set(ADCSRA,ADIF);
					clear(ADCSRA,ADEN);	
				
					//set channel selection to F6 for puck possession
					set(ADMUX,MUX0);
					set(ADMUX,MUX1);
					set(ADMUX,MUX2);
					clear(ADCSRB,MUX5);	
	
					set(ADCSRA,ADEN);
					set(ADCSRA,ADSC);
						
					while(!check(ADCSRA,ADIF));
						
					if(ADC >= 960){possession = true; m_red(ON);}
					if(ADC < 960){possession = false;}
				}
			}

				if(possession && !wireless_buffer_f)
				{
					wireless_buffer_f = false;

					buffer[0] = 1;
					send_message_to_bot(buffer, 0x24);	

					m_green(ON);

					if(theta > theta_goal + 0.125){set_left(35); set_right(25); m_red(OFF); m_green(ON); sit_1 = true;}
					if(theta < theta_goal - 0.125){set_left(25); set_right(35); m_red(ON); m_green(OFF); sit_2 = true;}
					if(!sit_1 && !sit_2){set_left(50); set_right(50); align = 1; m_red(OFF); m_green(OFF);}

					sit_1 = false;
					sit_2 = false;
				}

			if(!possession){buffer[0] = 2; send_message_to_bot(buffer, 0x24);}
	
			set(ADCSRA,ADIF);
			clear(ADCSRA,ADEN);

			m_red(OFF);
			m_green(OFF);
			break;

			//case 0xA2 goes to default (GOAL A)

			//case 0xA3 goes to default (GOAL B)

			//case 0xA4 goes to default (PAUSE)

			//case 0xA5 goes to default (DETANGLE)

			case 0xA6: //halftime
			set_left(0);
			set_right(0);
			second_half = true;
			start = true;
			wireless_buffer_f = false;
			while(!wireless_buffer_f);
			break;

			//case 0xA7 goes to default (GAMEOVER)

			default:
			set_left(0);
			set_right(0);
		}
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


#endif
