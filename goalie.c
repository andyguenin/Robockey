#include "goalie.h"

void goalie{

  int puck_status = 0;

  // set voltage reference to 5V
  clear(ADMUX, REFS1);
  set(ADMUX, REFS0);

  m_disableJTAG();//allowing gpio of F pins

  //setting ADC prescaler
  set(ADCSRA,ADPS2);
  set(ADCSRA,ADPS1);
  set(ADCSRA,ADPS0);

  //setting pins to turn off digital circuitry
  set(DIDR0,ADC4D);//setting F4
  set(DIDR0,ADC6D);//setting F6
  set(DIDR2,ADC8D);//setting D4
  set(DIDR2,ADC9D);//setting D6

  set(ADCSRA,ADATE);//setting triggering to free running
  
  //setting pins for output & setting pins high to supply power to phototransistors
  set(DDRF,5);//F5
  set(PORTF,5);
  set(DDRF,7);//F7
  set(PORTF,7);
  set(DDRB,3);//B3
  set(PORTB,3);
  set(DDRD,3);//D3
  set(PORTD,3);

  set(DDRF,0);//setting  pins to turn the sprinner motors
  set(PORTF,0);
  set(DDRF,1);
  set(PORTF,1);

  int adc = 0;//declaring integer for adc status;
  int adc_values[4];//declaring adc array
  
  m_usb_init();//initializing usb communication
  init_all();//initializing all the rest
  wait(2);
  m_red(ON);
  m_green(ON);
  wait(2);
  m_green(OFF);
	
  m_red(OFF);
  m_green(OFF);
  wait(2);

  while(1){
    //F4 for acd conversion
    clear(ADCSRA,ADEN);
    clear(ADCSRB,MUX5);//setting mux to F4
    set(ADMUX,MUX2);
    clear(ADMUX,MUX1);
    clear(ADMUX,MUX0);

    //starting adc for F4
    set(ADCSRA,ADEN);
    set(ADCSRA,ADSC);

    while(!check(ADCSRA,ADIF));
    adc_values[1] = ADC;//saving adc value
    set(ADCSRA,ADIF);//clearing flag

  
    //F6 for acd conversion
    clear(ADCSRA,ADEN);
    clear(ADCSRB,MUX5);//setting mux to F6
    set(ADMUX,MUX2);
    set(ADMUX,MUX1);
    clear(ADMUX,MUX0);

    //starting adc for F6
    set(ADCSRA,ADEN);
    set(ADCSRA,ADSC);

    while(!check(ADCSRA,ADIF));
    adc_values[0] = ADC;//saving adc value
    set(ADCSRA,ADIF);//clearing flag
  
    clear(ADCSRA,ADEN);
    //D6 for acd conversion
    set(ADCSRB,MUX5);//setting mux to D6
    clear(ADMUX,MUX2);
    clear(ADMUX,MUX1);
    set(ADMUX,MUX0);

    //starting adc for D6
    set(ADCSRA,ADEN);
    set(ADCSRA,ADSC);

    while(!check(ADCSRA,ADIF));
    adc_values[2] = ADC;//saving adc value
    set(ADCSRA,ADIF);//clearing flag

    //D4 for acd conversion
    clear(ADCSRA,ADEN);
    set(ADCSRB,MUX5);//setting mux to D4
    clear(ADMUX,MUX2);
    clear(ADMUX,MUX1);
    clear(ADMUX,MUX0);

    //starting adc for D4
    set(ADCSRA,ADEN);
    set(ADCSRA,ADSC);

    while(!check(ADCSRA,ADIF));
    adc_values[3] = ADC;//saving adc value
    set(ADCSRA,ADIF);//clearing flag

    puck_status = 0;

    if(adc_values[0]>700  && adc_values[0]>adc_values[2] && adc_values[0]>adc_values[3])
      puck_status = 2;

    if(adc_values[1]>800 && (adc_values[1]-adc_values[2] < -100 || adc_values[1]-adc_values[2] > 100) && adc_values[1]>adc_values[3])
      puck_status = 2;

    if(adc_values[1]>800 && (adc_values[1]-adc_values[2] > -100 && adc_values[1]-adc_values[2] < 100))
      puck_status = 0;

    if(adc_values[2]>800 && (adc_values[1]-adc_values[2] < -100 || adc_values[1]-adc_values[2] > 100) && adc_values[2]>adc_values[1])
      puck_status = 1;   

    if(adc_values[3]>800  && adc_values[3]>adc_values[0] && adc_values[3]>adc_values[1])
      puck_status = 1;
    
    switch(puck_status){
      case 1:
        set_left(90);
        set_right(50);
        m_red(ON);
        m_green(ON);
        break;

      case 2:
        set_left(-90);
        set_right(-50);
        m_red(ON);
        m_green(OFF);
        break;

      case 0:
        set_left(0);
        set_right(0);
        m_red(OFF);
        m_green(ON);
        break;
    }
    if(m_usb_isconnected()){
      //print out adc values to screen
      m_red(TOGGLE);
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
      m_wait(1000);
    }
      
  }
		
	
}
