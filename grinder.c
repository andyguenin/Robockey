                                                                     
                                                                     
                                                                     
                                             
#include "common_robockey.h"
#ifdef GRINDER

unsigned int blobs[12];

double x;
double y;
double theta;

double theta_goal;

double fix_theta(double);

char buffer[12];

unsigned int i = 0;

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

		switch(inst)
		{
			case 1:
			inst = 0xA1;
			break;

			case 2:
			inst = 0xA1;
			break;

			case 0xA0: //command test
			
			m_green(ON); m_red(ON); m_wait(300);
			m_green(OFF); m_red(OFF); m_wait(300);
			m_green(ON); m_red(ON);	m_wait(300);
			m_green(OFF); m_red(OFF); 

			while(!wireless_buffer_f);
			wireless_buffer_f = false;
			
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

			while(start && !wireless_buffer_f)
			{
				wireless_buffer_f = false;
				if(ADC >= 870){set(ADCSRA,ADIF); set(ADCSRA,ADSC); set_left(100); set_right(100); m_green(ON);}
				if(ADC < 870){set(ADCSRA,ADIF); set(ADCSRA,ADSC); set_left(-50); set_right(-50); m_green(OFF); start = false; m_wait(500); set_left(30); set_right(-30); m_wait(500);}			
			}

			start = false;

			m_wii_read(blobs);
			get_position(blobs, &x, &y, &theta);
			theta = fix_theta(theta);

			if(ADC < 930 && !target)
			{
				m_green(OFF);				

				if(i < 2000){set_left(8); set_right(24); i = i + 1;}
				if(i == 2000){set_left(-50); set_right(-50); m_wait(500); i = i + 1;}
				if(i > 4000 && i < 5000){set_left(24); set_right(8); i = i + 1;}
				if(i == 5000){set_left(-50); set_right(-50); m_wait(500); i = 0;}

				target = false;
			}

			if(ADC < 930 && target)
			{
				set_left(-50);
				set_right(-50);

				m_wait(500);

				set_left(0);
				set_right(0);

				target = false;
			}

			if(ADC >= 930 && !target){m_green(ON); set_left(90); set_right(90); i = 0; target = true;}

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
					
				if(ADC < 930){possession = false;}
				if(ADC >= 930)
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
						
					if(ADC >= 930){possession = true; m_red(ON);}
					if(ADC < 930){possession = false;}
				}
			}

				if(possession && !wireless_buffer_f)
				{
					wireless_buffer_f = false;

					buffer[0] = 1;
					send_message_to_bot(buffer, 0x24);	

					if(theta > theta_goal + 0.79){set_left(35); set_right(15); sit_1 = true;}
					if(theta < theta_goal - 0.79){set_left(15); set_right(35); sit_2 = true;}
					if(!sit_1 && !sit_2){set_left(100); set_right(100);}

					sit_1 = false;
					sit_2 = false;
				}

			if(!possession){buffer[0] = 2; send_message_to_bot(buffer, 0x24); m_red(OFF);}
	
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

			theta_goal = theta_goal + pi;
			theta_goal = fix_theta(theta_goal);

			start = true;
			i = 0;


			wireless_buffer_f = false;
			while(!wireless_buffer_f);

			break;


			//case 0xA7 goes to default (GAMEOVER)

			default:
			
			set_left(0);
			set_right(0);

			start = true;
			i = 0;

			wireless_buffer_f = false;
			while(!wireless_buffer_f);
			
			break;
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
