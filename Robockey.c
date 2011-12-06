#include "common_robockey.h"
#include "bot.h"

int main(void)
{
	#ifdef POS_DEMO
	position_demo();
	#endif
	
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

