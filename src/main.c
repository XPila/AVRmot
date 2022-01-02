// main.c
#include <stdio.h>
#include <avr/pgmspace.h>
#include "cmd.h"
#include "mmctl/mmctl.h"


// initialization after reset
void setup(void)
{
	mmctl_init();
	fprintf_P(cmd_err, PSTR("start\n")); //startup message
	fflush(cmd_err);
}

#include "timer0.h"
// loop function
void loop(void)
{
	cmd_process();
//	if ((timer0_ms % 1000) == 0)
//		fprintf_P(cmd_err, PSTR("%lu\n"), timer0_ms); //startup message
}

// main
int main(void)
{
	setup();
	while (1)
		loop();
}
