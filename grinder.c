#include "common_robockey.h"

#ifdef GRINDER

double theta_desired = 0;

int puck_status = 0;
char input[12];
char get_transition(void);

//initializing position variables
unsigned int blobs[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
double x;
double x_center;
double y;
double y_center;
double theta;
double theta_zero;

//Game command states
void state_before_game(void);
void state_play(void);
void state_pause(void);
void state_detangle(void);

int adc_values[12];

void grinder(){
  init_all();//initializing all the rest

  //displaying two flashes to know robot is ready and waiting for start
  m_red(ON);
  m_green(ON);
  m_wait(500);
  m_green(OFF);
  m_red(OFF);
	m_wait(500);
  m_red(ON);
  m_green(ON);
  m_wait(500);
  m_red(OFF);
  m_green(OFF);

  //initializing center ice
  set_position(1024/2,768/2);
  get_position(blobs, &x_center, &y_center, &theta_zero);

  // set voltage reference to 5V
  clear(ADMUX, REFS1);
  set(ADMUX, REFS0);

  m_disableJTAG();//allowing gpio of F pins

  //setting ADC prescaler
  set(ADCSRA,ADPS2);
  set(ADCSRA,ADPS1);
  set(ADCSRA,ADPS0);

  //setting pins to turn off digital circuitry
  set(DIDR2,ADC10D);//setting D6

  set(ADCSRA,ADATE);//setting triggering to free running

  int adc = 0;//declaring integer for adc status;
  int adc_values[4];//declaring adc array
    
	//state_before_game();
  state_play();//just for testing now switch back after
}

void state_before_game()
{ 		
  m_red(ON);
	while(!wireless_buffer_f);
	m_red(OFF);
	wireless_buffer_f = false;
	char inst = wireless_buffer[0];
	switch(wireless_buffer[0]) 
	{
		case 0xA0:
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

void state_detangle()
{
	m_red(ON);
	set_left(-90);
	set_right(-50);
	m_wait(2000);
	m_red(OFF);
	set_left(0);
	set_right(0);
	char transition = wireless_buffer[0];
	switch(transition)
	{
		case 0xA1:
			state_play();
			return;
	}
}

void state_play()
{
	    //D7 for acd conversion
	    clear(ADCSRA,ADEN);
	    set(ADCSRB,MUX5);//setting mux to D7
	    clear(ADMUX,MUX2);
	    set(ADMUX,MUX1);
	    clear(ADMUX,MUX0);

	    //starting adc for D7
	    set(ADCSRA,ADEN);
	    set(ADCSRA,ADSC);

	    while(!check(ADCSRA,ADIF));
	    adc_values[0] = ADC;//saving adc value
	    set(ADCSRA,ADIF);//clearing flag

	    puck_status = 0;

	    if(adc_values[0]>700)
	      puck_status = 1;

	    switch(puck_status){
	      case 1:
	        set_left(50);
	        set_right(50);
	        m_red(ON);
	        m_green(ON);
	        break;

	      case 0:
	        set_left(-30);
	        set_right(30);
	        m_red(ON);
	        m_green(OFF);
	        break;


	      default:
	        set_left(-30);
	        set_right(30);
	        m_red(ON);
	        m_green(OFF);
	        break;
	    }

      get_position(blobs, &x, &y, &theta);

	    if(m_usb_isconnected()){
	      //print out adc values to screen
	      /*m_red(TOGGLE);
	      m_usb_tx_string("Phototrans 1 = ");
	      m_usb_tx_int((int) adc_values[0]);
	      m_usb_tx_string("\n");
	      m_usb_tx_string("Phototrans 2 = ");
		  	m_usb_tx_int((int)adc_values[1]);
	      m_usb_tx_string("\n");
	      m_usb_tx_string("Phototrans 3 = ");
		  	m_usb_tx_int((int)adc_values[2]);
	      m_usb_tx_string("\n");
	      m_usb_tx_string("Phototrans 4 = ");
		  	m_usb_tx_int((int)adc_values[3]);
	      m_usb_tx_string("\n");
	      m_usb_tx_string("puck status = ");
		  	m_usb_tx_int((int)puck_status);
	      m_usb_tx_string("\n");
	      m_wait(1000);*/
        m_usb_tx_string("X = ");
	      m_usb_tx_int((int) x-x_center);
	      m_usb_tx_string("\n");
        m_usb_tx_string("Y = ");
	      m_usb_tx_int((int) y-y_center);
	      m_usb_tx_string("\n");
        m_usb_tx_string("Theta = ");
	      m_usb_tx_int((int) theta-theta_zero);
	      m_usb_tx_string("\n");
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
    case 0xA5:
      state_detangle();
      return;
		default:
			state_play();
			return;
	}
}


void state_pause()
{
	set_left(0);
	set_right(0);
  while(!wireless_buffer_f);
	wireless_buffer_f = false;
	char inst = wireless_buffer[0];
	switch(inst)
	{
		case 0xA4:
			state_pause();
			return;
		case 0xA1:
			state_play();
			return;
    case 0xA5:
			state_detangle();
			return;
		default:
			state_pause();
			return;
	}	
}

			
#endif
