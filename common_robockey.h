#include "bot.h"
#include "m_general.h"
#include "communication.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_wii.h"
#include "m_usb.h"
#include "position.h"
#include "demo.h"
#include "movement.h"
#include <math.h>
#ifdef SNIPER
#include "sniper.h"
#define ADDRESS 0x24
#endif
#ifdef GRINDER
#include "grinder.h"
#define ADDRESS 0x25
#endif
#ifdef GOALIE
#include "goalie.h"
#define ADDRESS 0x26
#endif

#define PACKET_SIZE 12
#define CHANNEL 1


#define pi 3.14159265358979323


void init_all();

void wait(int);

void set_left(int);
void set_right(int);




bool wireless_buffer_full(void);
void get_wireless_buffer(char*);

//char arr[PACKET_SIZE];
volatile char wireless_buffer[PACKET_SIZE];
