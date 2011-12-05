#include "common_robockey.h"

#ifdef GRINDER

double theta_desired = 0;

unsigned int blobs[12];
double x_shoot = 0;
double y_shoot = 768;
double x_screen1 = 100;
double y_screen1 = 768;
double x_screen2 = -100;
double y_screen2 = 768;

void grinder(void)
{

	init_all();

	double x;
	double y;
	double theta;
	
	set_left(0);
	set_right(0);

    //set voltage reference to 5V
    clear(ADMUX, REFS1);
    set(ADMUX, REFS0);

    m_disableJTAG();//allowing gpio of F pins

    //set ADC prescaler
    set(ADCSRA,ADPS2);
    set(ADCSRA,ADPS1);
    set(ADCSRA,ADPS0);

    //set pin to turn off digital circuitry
    set(DIDR2,ADC10D);
    
    //set channel selection to D7 for puck location
    clear(ADMUX,MUX0);
    set(ADMUX,MUX1);
    clear(ADMUX,MUX2);
    set(ADCSRB,MUX5);
    
    //set B0 for puck possession input
    clear(DDRB,0);

    //initialize position at center ice
    m_red(ON); m_green(ON); m_wait(300);
    m_red(OFF); m_green(OFF); m_wait(300);
    m_red(ON); m_green(ON); m_wait(300);
    
    m_wii_read(blobs);
    set_position(1024/2,768/2);
    get_position(blobs, &x, &y, &theta);
    
    m_green(OFF); m_red(OFF); m_wait(300);
    m_red(ON); m_green(ON); m_wait(300);
    m_red(OFF); m_green(OFF); m_wait(300);

    //wait for command
 	//while(!wireless_buffer_f){m_green(ON);}
 	//wireless_buffer_f = false;
    char inst = wireless_buffer[0];
 	//m_green(OFF);
    
//TESTING
/*
	while(1)
	{
		m_red(ON);
	    set(ADCSRA,ADEN);
	    set(ADCSRA,ADSC);
	    while(!check(ADCSRA,ADIF));
	    if(ADC < 800){m_green(ON); set_left(-15); set_right(15);}
	    if(ADC >= 800){m_green(OFF); set_left(30); set_right(30);}
	    if(check(PINB,0))
	       {
	           m_green(ON);
			   send_message_to_bot((char*)2, 0x24);
	           m_wii_read(blobs);
	           get_position(blobs, &x, &y, &theta);
	           make_positive(&theta);
	           theta_desired = atan2(x_shoot-x,y_shoot-y);
			   while(theta > theta_desired * 11 / 10 && theta < theta_desired * 9 / 10)
	           {
	           		if(theta > theta_desired){set_left(20); set_right(10);}
	           		if(theta < theta_desired){set_left(10); set_right(20);}
	           		m_wii_read(blobs);
	           		get_position(blobs, &x, &y, &theta);
	           		make_positive(&theta);
	           		theta_desired = atan2(x_shoot-x,y_shoot-y);
	           }
	        m_green(OFF);
			set_left(0);
	    	set_right(0);
	    }
		set(ADCSRA,ADIF);
	}
*/
//END TESTING


    while(1)
    {    
		if(wireless_buffer_f)
		{
			wireless_buffer_f = false;
			inst = wireless_buffer[0];
		    switch(inst)
		    {
		        case 1: //sniper has puck
		        /*
				m_wii_read(blobs);
		        get_position(blobs, &x, &y, &theta);
		        make_positive(&theta);
		        theta_desired = atan2(100-x,768-y);
		        while(theta != theta_desired)
		        {
		            if(theta > theta_desired){set_left(20); set_right(10);}
		            if(theta < theta_desired){set_left(10); set_right(20);}
		            m_wii_read(blobs);
		            get_position(blobs, &x, &y, &theta);
		            make_positive(&theta);
		            theta_desired = atan2(100-x,768-y);
		        }
		        theta_desired = atan2(-100-x,768-y);
		        while(theta != theta_desired)
		        {
		            if(theta > theta_desired){set_left(20); set_right(10);}
		            if(theta < theta_desired){set_left(10); set_right(20);}
		            m_wii_read(blobs);
		            get_position(blobs, &x, &y, &theta);
		            make_positive(&theta);
		            theta_desired = atan2(-100-x,768-y);
		        }
		        send_message_to_bot((char*)3,0x24);
				*/		 
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
		        while(!wireless_buffer_f);
				wireless_buffer_f = false;
				break;
                    
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
			    if(ADC < 800){m_green(ON); set_left(-15); set_right(15);}
			    if(ADC >= 800){m_green(OFF); set_left(30); set_right(30);}
			    if(check(PINB,0))
			       {
			           m_green(ON);
					   send_message_to_bot((char*)2, 0x24);
			           m_wii_read(blobs);
			           get_position(blobs, &x, &y, &theta);
			           make_positive(&theta);
			           theta_desired = atan2(x_shoot-x,y_shoot-y);
					   while(theta > theta_desired * 11 / 10 && theta < theta_desired * 9 / 10)
			           {
			           		if(theta > theta_desired){set_left(20); set_right(10);}
							if(theta < theta_desired){set_left(10); set_right(20);}	      
				       		m_wii_read(blobs);
			           		get_position(blobs, &x, &y, &theta);
			           		make_positive(&theta);
			           		theta_desired = atan2(x_shoot-x,y_shoot-y);
			           }
			        m_green(OFF);
					set_left(0);
			    	set_right(0);
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
        
		        case 0xA6: //halftime
				x_shoot = 0;
				y_shoot = 0;
				break;
                    
		        //case 0xA7 goes to default (GAMEOVER)
        
		        default:
		        set_left(0);
		        set_right(0);
		        break;
    
		    }
    
		}
    }    
}

#endif
