#include "bot.h"
#include "m_general.h"
#include "communication.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_wii.h"
#include "m_usb.h"
#include "position.h"
#include "demo.h"
#include <math.h>
#ifdef SNIPER
#include "sniper.h"
#define ADDRESS 0x24
#elif GRINDER
#include "grinder.h"
#define ADDRESS 0x25
#elif GOALIE
#include "goalie.h"
#define ADDRESS 0x26
#else
#define ADDRESS 0x24
#endif

#define PACKET_SIZE 12
#define CHANNEL 1




void init_all();

void wait(int);

void set_left(int);
void set_right(int);


void wireless_callback(char* c);
bool wireless_buffer_full(void);
char* get_wireless_buffer(void);
