#ifdef GRINDER

#include "common_robockey.h"

void init_grinder(void);

void grinder(void)
{
	
	init_all();

	init_grinder();
	//wait for command
	while(!wireless_buffer_f){m_green(ON);}
	wireless_buffer_f = false;
	char inst = wireless_buffer[0];
	m_green(OFF);
	
	while(1)
	{	

		switch(inst)
		{
			case 1: //sniper has puck
			//screen goalie
			//signal goalie screen complete
			send_message_to_bot((char*)3,0x24);
			break;
			
			case 2: //grinder has puck
			break;
			
			case 3: //grinder screening goalie
			break;
			
			case 4: //sniper screening goalie
			//push puck towards goal
			break;
			
			case 5: //sniper lost puck

			break;
			
			case 6: //wait for screen
			while(!wireless_buffer_full());
						
			case 0xA0: //command test
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
			break;
			
			case 0xA1: //play
			m_red(ON);
			set(ADCSRA,ADEN);
			set(ADCSRA,ADSC);
			while(!check(ADCSRA,ADIF));
			if(ADC < 990){m_green(ON); set_left(-20); set_right(20);}
			if(ADC >= 990){m_green(OFF); set_left(60); set_right(60);}
			if(check(PINB,0))
			{
				send_message_to_bot((char*)2, 0x24);
				m_wii_read(blobs);
				get_position(blobs, &x, &y, &theta);
				while(x > 537 || x < 487){set_right(20); set_left(0);}
				while(y > 409 || y <359){set_right(20); set_left(20);}
				while(theta > 6.28 || theta < 0)
				{
					if(theta > 6.28){theta = theta - 3.14;}
					if(theta < 0){theta = theta + 3.14;}
				}
				while(theta > drive_net + .2 || theta < drive_net - .2){set_right(0); set_left(20);}
				//aligned with net (potentially wait for screen signal)
				set_left(20);
				set_right(20);
			}
			set(ADCSRA,ADIF);
			break;
			
			//case 0xA2 goes to default (GOAL A)
			
			//case 0xA3 goes to default (GOAL B)
			
			//case 0xA4 goes to default (PAUSE)
			
			case 0xA5: //detangle
			set_left(-15);
			set_right(-15);
			break;
			
			//case 0xA6 goes to default (HALFTIME)
						
			//case 0xA7 goes to default (GAMEOVER)
			
			default:
			set_left(0);
			set_right(0);
			break;
		
		}
		
		if(wireless_buffer_f)
		{
			inst = wireless_buffer[0];

		}

	}	
}

void init_grinder(void)
{

	//set voltage reference to 5V
	clear(ADMUX, REFS1);
	set(ADMUX, REFS0);

	m_disableJTAG();//allowing gpio of F pins

	//set ADC prescaler
	set(ADCSRA,ADPS2);
	set(ADCSRA,ADPS1);
	set(ADCSRA,ADPS0);

	//set pin to turn off digital circuitry
	set(DIDR0,ADC7D);
	
	//set channel selection to D7 for puck location
	clear(ADMUX,MUX0);
	set(ADMUX,MUX1);
	clear(ADMUX,MUX2);
	set(ADCSRB,MUX5);
	
	//set B0 for puck possession input
	clear(DDRB,0);

	//initialize position at center ice
	m_red(ON); m_green(ON); wait(1);
	m_red(OFF); m_green(OFF); wait(1);
	m_red(ON); m_green(ON); wait(1);
	
	m_wii_read(blobs);
	set_position(1024/2,768/2);
	get_position(blobs, &x, &y, &theta);
	double drive_net = theta;
	
	m_green(OFF); m_red(OFF); wait(1);
	m_red(ON); m_green(ON); wait(1);
	m_red(OFF); m_green(OFF); wait(1);
}

#endif
