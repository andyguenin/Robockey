volatile bool wireless_buffer_f;



void init_communication(int c, int a, int l);
void send_message(char* c);
void send_message_to_bot(char* c, char address);


#define COMMTEST	0xA0
#define PLAY		0xA1
#define GOALA		0xA2
#define GOALB		0xA3
#define PAUSE		0xA4
#define DETANGLE	0xA5
#define HALFTIME	0xA6
#define GAMEOVER	0xA7
