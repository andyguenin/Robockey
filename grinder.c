#include "common_robockey.h"

volatile char = value;

void grinder()
{

  m_bus_init();
	m_rf_init();
	
	init_all();

	// set voltage reference to 5V
	clear(ADMUX, REFS1);
	set(ADMUX, REFS0);

	m_disableJTAG();//allowing gpio of F pins

	//set ADC prescaler
	set(ADCSRA,ADPS2);
	set(ADCSRA,ADPS1);
	set(ADCSRA,ADPS0);

	//set pin to turn off digital circuitry
	set(DDRO,ADC7D);
	
	//set trigger to free running mode
	set(ADCSRA,ADATE);
	
	//set channel selection
	set(ADMUX,MUX0);
	set(ADMUX,MUX1);
	set(ADMUX,MUX2);
	clear(ADCSRB,MUX5);
	
	int ADC = 0; 	
	int puck = 0;

	m_red(ON);
	wait(2);
	set_position();
	m_red(OFF);

	while(!wireless_buffer_full())
	{
		m_green(ON);
	}
	
	m_green(OFF);
	
	while(1)
	{
		switch(value)
		{
			case 1: //sniper has puck
			go_to(x,y,t,s); //screen goalie
			m_rf_send(0x26,3,1);
			break;
			
			case 2: //grinder has puck
			break;
			
			case 3: //grinder screening goalie
			break;
			
			case 4: //sniper screening goalie
			go_to(x,y,t,s) //push puck towards goal
			break;
			
			case 5: //sniper lost puck
			value = 0xA1;
			break;
			
			case 6: //wait for screen
			while(!wireless_buffer_full());
						
			case 0xA0:
			m_red(ON);
			m_wait(500);
			m_red(OFF);
			break;
			
			case 0xA1:
			set(ADCSRA,ADEN);
			set(ADCSRA,ADSC);
			while(!check(ADCSRA,ADIF));
			puck = ADC;
			if(puck < 900){set_left(1); set_right(-1);}
			else{set_left(50); set_right(50);}
			if(check(PINB,0))
			{
				m_rf_send(0x26,2,1);
				go_to(x,y,t,s); //wait for screen
				value = 6;
			}
			break;
			
			case 0xA2:
			set_left(0);
			set_right(0);
			break;
			
			case 0xA3:
			set_left(0);
			set_right(0);
			break;
			
			case 0xA4:
			set_left(0);
			set_right(0);
			break;
			
			case 0xA5:
			set_left(-1);
			set_right(-1);
			break;
			
			case 0xA6:
			set_left(0);
			set_right(0);
			break;
						
			case 0xA7:
			set_left(0);
			set_right(0);
			break;
			
			default:
			value = 0xA1;
			break;
			
	}

}

ISR(INT2_vect)
{
	wireless_buffer_full(void);
	value = char* get_wireless_buffer(void);
}
