//sys.c

#include "sys.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


int8_t sys_state = 0;
uint8_t sys_signals = 0;


void sys_reset(void)
{
	wdt_enable(WDTO_15MS);
	cli();
	while(1);
}

void sys_init_wdt(void)
{
	uint8_t _sreg = SREG;
	cli();
	wdt_reset();
	MCUSR &= ~(1 << WDRF);
	WDTCSR = (1 << WDCE) | (1 << WDE);
	WDTCSR = 0x00;
	wdt_disable();
	SREG = _sreg;
}

void sys_setup_osc(void)
{
#ifdef CLKSEL0
	CLKSEL0 = 0x15;    //Choose Crystal oscillator with BOD
	CLKSEL1 = 0x0f;    //CLKSEL1.EXCKSEL0..3 = 1;
	CLKPR = 0x80;      //Change the clock prescaler, first change bit CLKPCE
	CLKPR = 0x00;      //
#endif //CLKSEL0
}

void sys_disable_usb(void)
{
#ifdef UHWCON
	UHWCON = 0;
	USBCON = 0;
#endif //UHWCON
}

#define bootKey 0x7777
volatile uint16_t *const bootKeyPtr = (volatile uint16_t *)0x0800;

void sys_bootloader(void)
{
	*bootKeyPtr = bootKey;
	wdt_enable(WDTO_15MS);
	cli();
#ifndef _SIMULATOR
	while(1);
#endif //_SIMULATOR
}
