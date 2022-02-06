// main.c

#include <stdio.h>
#include <avr/pgmspace.h>
#include "cmd.h"
#include "einsy.h"

#include "st4.h"
#include "tmc2130_hw.h"


// initialization after reset
void setup(void)
{
	einsy_init();
	fprintf_P(cmd_err, PSTR("start\n")); //startup message
	fflush(cmd_err);
//	st4_fprint_sr_d2(cmd_err,32,22000);
//	st4_mor(0, 1000);
//	st4_mor(1, 1000);
}

#include "timer0.h"
// loop function
void loop(void)
{
	cmd_process();
#if 1
	if (tmc2130_ena & 1)
		if ((st4_msk & 1) == 0)
		{
			if (st4_axis[0].pos >= 0)
				st4_mor(0, -10000);
			else
				st4_mor(0, 10000);
		}
	if (tmc2130_ena & 2)
		if ((st4_msk & 2) == 0)
		{
			if (st4_axis[1].pos >= 0)
				st4_mor(1, -10000);
			else
				st4_mor(1, 10000);
		}
#endif
//	st4_cycle();
}

// main
int main(void)
{
	setup();
	while (1)
		loop();
}
