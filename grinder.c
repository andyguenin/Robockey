#include "common_robockey.h"


int value = 0;
unsigned int blobs[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
double x;
double y;
double theta;


void grinder(void)
{
	
	init_all();

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
	m_red(ON);
	wait(2);
	m_wii_read(blobs);
	set_position(1024/2,768/2);
	m_red(OFF);

//ADDITIONAL STUFF
/*
	wait(2);
	m_green(ON);

	while(1)
	{
		set(ADCSRA,ADEN);
		set(ADCSRA,ADSC);
		while(!check(ADCSRA,ADIF));
			if(ADC < 990){m_green(ON); set_left(-15); set_right(15);}
			if(ADC >= 990){m_green(OFF); set_left(15); set_right(15);}
			if(check(PINB,0))
			{
				send_message_to_bot((char*)2, 0x24);
				m_wii_read(blobs);
				get_position(blobs, &x, &y, &theta);
				while(x > 537 || x < 587){set_right(10); set_left(-10);}
				while(y > 409 || y <359){set_right(10); set_left(10);}
				set_left(0);
				set_right(0);
			}
			set(ADCSRA,ADIF);
	}
*/
//END ADDITIONAL STUFF

	//wait for command
	while(!wireless_buffer_full()){m_green(ON);}
	char in[12];
	get_wireless_buffer(in0);
	value = (int)in[0];
	m_green(OFF);
	
	while(1)
	{	

//		m_wii_read(blobs);
//		get_position(blobs, &x, &y, &theta);
		
		switch(value)
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
			value = 0xA1;
			break;
			
			case 6: //wait for screen
			while(!wireless_buffer_full());
						
			case 0xA0: //command test
			m_red(ON);
			m_wait(500);
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
				while(x > 537 || x < 587){set_right(20); set_left(-20);}
				while(y > 409 || y <359){set_right(20); set_left(20);}
				set_left(0);
				set_right(0);
			}
			set(ADCSRA,ADIF);
			break;
			
			case 0xA2: //goal a
			set_left(0);
			set_right(0);
			break;
			
			case 0xA3: //goal b
			set_left(0);
			set_right(0);
			break;
			
			case 0xA4: //pause
			set_left(0);
			set_right(0);
			break;
			
			case 0xA5: //detangle
			set_left(-15);
			set_right(-15);
			break;
			
			case 0xA6: //halftime
			set_left(0);
			set_right(0);
			break;
						
			case 0xA7: //gameover
			set_left(0);
			set_right(0);
			break;
			
			default:
			value = 0xA1;
			break;
		
		}
		
		if(wireless_buffer_full())
		{
			value = (int)get_wireless_buffer();

		}

	}	
}
