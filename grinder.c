//git commit -am "______"

//git pull/push origin master

#include "common_robockey.h"

#ifdef GRINDER

unsigned int blobs[12];

double x;
double y;
double theta;

bool start = true;
bool possession = false;
bool target = false;

void grinder(void)
{

init_all();

set_left(0);
set_right(0);

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

    //initialize position at center ice
    m_red(ON); m_green(ON); m_wait(300);
    m_red(OFF); m_green(OFF); m_wait(300);
    m_red(ON); m_green(ON); m_wait(300);
    
    m_wii_read(blobs);
    set_position(512,384);
    get_position(blobs, &x, &y, &theta);
    
    m_green(OFF); m_red(OFF); m_wait(300);
    m_red(ON); m_green(ON); m_wait(300);
    m_red(OFF); m_green(OFF); m_wait(300);

    char inst = wireless_buffer[0];

    while(1)
    {    

    //TEST SEGMENT
    wait(1);
    inst = 0xA1;
    wireless_buffer_f = false;
    //TEST SEGMENT

    /*
    if(wireless_buffer_f)
    {
        wireless_buffer_f = false;
        inst = wireless_buffer[0];
*/  switch(inst)
    {
        case 1: //sniper has puck  
        break;
        
        case 2: //grinder has puck
        break;
        
        case 3: //grinder screening goalie
        break;

        case 4: //sniper screening goalie
        break;
        
        case 5: //sniper lost puck
        break;
        
        case 6: //wait for screen
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
        m_red(ON);
    
        //set channel selection to D7 for puck location
/*        clear(ADMUX,MUX0);
        set(ADMUX,MUX1);
        clear(ADMUX,MUX2);
        set(ADCSRB,MUX5);   
    
        set(ADCSRA,ADEN);
        set(ADCSRA,ADSC);
*/
/*        if(start)
        {
            start = false;
            while(!check(ADCSRA,ADIF));
            while(ADC > 800)
            {
                set_left(100);
                set_right(100);
                
                set(ADCSRA,ADIF);
                set(ADCSRA,ADSC);
                
                while(!check(ADCSRA,ADIF));
            }
        }
*/        
/*        while(!check(ADCSRA,ADIF));
        if(ADC < 975){m_green(ON); set_left(-15); set_right(15); target = false;}
        if(ADC >= 975){m_green(OFF); set_left(30); set_right(30); target = true;}
    
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
            if(ADC < 1010){possession = false;}
            if(ADC >= 1010)
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
                if(ADC >= 1010){possession = true;}
                if(ADC < 1010){possession = false;}
            }
        }        
*/    
//        if(possession && !wireless_buffer_f)
//        {
//         wireless_buffer_f = false;
//        m_green(ON);
       
//        send_message_to_bot((char*)2, 0x24);
       
        m_wii_read(blobs);
        get_position(blobs, &x, &y, &theta);
       
        if(y > 359){set_left(20); set_right(10); m_red(OFF); m_green(ON);}
        if(y < 409){set_left(10); set_right(20); m_red(ON); m_green(OFF);}
        if(y >= 359 && y <= 409){set_left(50); set_right(50); m_red(OFF); m_green(OFF);} 
   
//        if(theta < 0.9 * theta_desired){set_left(20); set_right(10);}
//        if(theta > 1.1 * theta_desired){set_left(10); set_right(20);}      
   
//        if(theta >= 0.9 * theta_desired && theta <= 1.1 * theta_desired){set_left(40); set_right(40);}
//       }
    
//        set(ADCSRA,ADIF);
//        m_red(OFF);    
//        m_green(OFF);
        break;
        
        //case 0xA2 goes to default (GOAL A)
        
        //case 0xA3 goes to default (GOAL B)
        
        //case 0xA4 goes to default (PAUSE)
        
        //case 0xA5 goes to defaul (DETANGLE)
        
        case 0xA6: //halftime
        break;
                    
        //case 0xA7 goes to default (GAMEOVER)
        
        default:
        set_left(0);
        set_right(0);
        break;
    
        }
    }
}    

//}

#endif

