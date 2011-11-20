#include "common_robockey.h"


char ADDRESS = 0x01;
char CHANNEL = 0X24;
char PACKET_SIZE = 1;



int main(void)
{

	init_all();
	while(1)
	{
		if(wireless_buffer_full())
		{
			char* wireless_buffer = get_wireless_buffer();

		}
	}
}



