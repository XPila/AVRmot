// main.c
#include <stdio.h>
#include <avr/pgmspace.h>
#include "cmd.h"
#include "mmctl.h"


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
}

// main
int main(void)
{
	setup();
	while (1)
		loop();
}
