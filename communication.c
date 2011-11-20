#include "common_robockey.h"

int channel, address, packet_length;

void init_communication(int c, int a, int l)
{
	m_bus_init();
	channel = c;
	address = a;
	packet_length = l;

	m_rf_open(channel, address, packet_length);
}


ISR(INT2_vect)
{
	char buffer;
	m_rf_read(&buffer, packet_length);
	wireless_callback(&buffer);
}

void send_message(char* c)
{
	for(int i = 0x24; i <= 0x26; i++)
	{
		if(address != i)
		{
			send_message_to_bot(c, i);

		}
	}
}

void send_message_to_bot(char* c, char address)
{
	m_rf_send(address, c, address);
}
