#include "common_robockey.h"
#include "bot.h"



volatile char* buffer;
volatile bool flag = false;

int main(void)
{

	
	#ifdef POS_DEMO
	position_demo();
	#endif
	
	#ifdef SNIPER
/*	init_all();
	m_usb_init();
	while(!m_usb_isconnected());
	m_green(ON);

	while(1)
	{
		while(!wireless_buffer_f);
		wireless_buffer_f = false;
		m_usb_tx_char(wireless_buffer[0]);
		m_usb_tx_char(wireless_buffer[1]);
		m_usb_tx_char(wireless_buffer[2]);
		m_usb_tx_char(wireless_buffer[3]);
		m_usb_tx_char(wireless_buffer[4]);
		m_usb_tx_char(wireless_buffer[5]);
		m_usb_tx_char(wireless_buffer[6]);
		m_usb_tx_char(wireless_buffer[7]);
		m_usb_tx_char(wireless_buffer[8]);
		m_usb_tx_char(wireless_buffer[9]);
		m_usb_tx_char(wireless_buffer[10]);
		m_usb_tx_char(wireless_buffer[11]);
		if(wireless_buffer[0] == 0xA0)
			m_red(ON);


		
	}*/

	sniper();
	#endif

	#ifdef GRINDER
	grinder();
	#endif

	#ifdef GOALIE
	/*init_all();
	char a[12] = {'a','b','c','d','e','f','a','b','c','d','e','f'};
	char b[12] = {'g','h','i','j','k','l','g','h','i','j','k','l'};
	while(1)
	{
		send_message_to_bot(a, 0x24);
		m_wait(2000);
		m_red(TOGGLE);
		send_message_to_bot(b, 0x24);
		m_wait(2000);
		m_red(TOGGLE);
	}
	*/
	goalie();	

	#endif

}

