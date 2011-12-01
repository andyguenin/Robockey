#include "common_robockey.h"
#include "bot.h"

int main(void)
{


	#ifdef POS_DEMO
	position_demo();
	#endif
	
	#ifdef SNIPER
	init_all();

	m_usb_init();
	while(!m_usb_isconnected());


	m_green(ON);
	char input[12];


	while(1)
	{
		while(!wireless_buffer_full());
		
		get_wireless_buffer(input);
		m_red(TOGGLE);
		m_usb_tx_char(input[0]);
		m_usb_tx_char(input[1]);
		m_usb_tx_char(input[2]);
		m_usb_tx_char(input[3]);
		m_usb_tx_char(input[4]);
		m_usb_tx_char(input[5]);
		m_usb_tx_char(input[6]);
		m_usb_tx_char(input[7]);
		m_usb_tx_char(input[8]);
		m_usb_tx_char(input[9]);
		m_usb_tx_char(input[10]);
		m_usb_tx_char(input[11]);
	}

	#endif

	#ifdef GRINDER
	grinder();
	#endif

	#ifdef GOALIE
	//goalie();

	init_all();
	char out[12];
	char a = 0;
	while(1)
	{
		for(int i = 0; i < 12; i++)
			out[i] = a;
		a++; 
		m_rf_send(0x24, out, 12);
		m_red(TOGGLE);
		//send_message_to_bot(out, 0x24);
		wait(2);
	}


	#endif

} 
