#include "common_robockey.h"


bool wireless_buffer_f;
char* wireless_buffer;
void wireless_callback(char*);

void init_all()
{
	m_clockdivide(0);
	m_disableJTAG();

	//initializing mWii
	char wii_status = m_wii_open();
	if(wii_status){
	  m_green(ON);
	}else
	  m_red(ON);
	
	
	//init_communication(CHANNEL, ADDRESS, PACKET_SIZE);

	
	// set timer clock source
	clear(TCCR1B, CS12);
	clear(TCCR1B, CS11);
	set(TCCR1B, CS10);

	// Set to up to ICR1 PWM mode
	ICR1 = 0xffff;
	set(TCCR1B, WGM13);
	set(TCCR1B, WGM12);
	set(TCCR1A, WGM11);
	clear(TCCR1A, WGM10);

	// set there to be an output at B5
	set(DDRB, 5);
	set(TCCR1A, COM1A1);
	clear(TCCR1A, COM1A0);

	// set there to be an output at B6
	set(DDRB, 6);
	set(TCCR1A, COM1B1);
	clear(TCCR1A, COM1B0);

	// don't use the one on pin B7
	clear(TCCR1A, COM1C1);
	clear(TCCR1A, COM1C0);

	OCR1A = 0;
	OCR1B = 0;

	
	// set the directions pins to enable output
	set(DDRC, 6);
	set(DDRC, 7);
	
	clear(PORTC, 6);
	clear(PORTC, 7);

	
}



void wait(int n)
{
	for(int i = 0; i < n; i++)
	{
		m_wait(1000);
	}
}

// B5 and C6
void set_left(int n)
{
	int pwm = n;
	if(n < 0)
	{
		pwm = -n;
		clear(PORTC, 6);
	}
	else
	{
		set(PORTC, 6);
	}
	OCR1A = pwm * 655;

}

	
// B6 and C7
void set_right(int n)
{
	int pwm = n;
	if(n < 0)
	{
		pwm = -n;
		clear(PORTC, 7);
	}
	else
	{
		set(PORTC, 7);
	}
	OCR1B = pwm * 655;
}

void wireless_callback(char* c)
{
	wireless_buffer = c;
	wireless_buffer_f = true;
}

bool wireless_buffer_full()
{
	return wireless_buffer_f;
}

char* get_wireless_buffer()
{
	wireless_buffer_f = false;
	return wireless_buffer;
	
}
