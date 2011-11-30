#include "m_usb.h"
#include "m_wii.h"
#include "m_general.h"
#include "position.h"
#include "common_robockey.h"
#include <math.h>
#include "bot.h"


int main(void)
{
	#ifdef SNIPER
	sniper();
	#endif

	#ifdef GRINDER
	grinder();
	#endif

	#ifdef GOALIE
	goalie();
	#endif

} 
