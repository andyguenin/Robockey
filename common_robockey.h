#include "robockey.h"
#include "m_general.h"
#include "communication.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_wii.h"
#include "m_usb.h"
#include <math.h>

void init_all();

void wait(int);

void set_left(int);
void set_right(int);



bool wireless_buffer_full(void);
char* get_wireless_buffer(void);
