// einsy.c

#include "einsy.h"
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "sys.h"
#include "timer0.h"
#include "uart.h"
#include "spi.h"
#include "adc.h"
#include "tmc2130.h"
#include "tmc2130_hw.h"
#include "st4.h"
#include "cmd.h"
#include "gpio_atmega2560.h"


uint16_t adc_value[2] = {0, 0};


void einsy_init(void)
{
	sys_init_wdt();

	sei();

	sys_disable_usb();

	sys_setup_osc();

	uart1_init();
	GPIO_SET_H(GPIO_PD2); // enable RX pin pullup
	stdin = uart1io; // stdin = uart1
	stdout = uart1io; // stdout = uart1
	cmd_in = uart1io;
	cmd_out = uart1io;
	cmd_err = uart1io;

#ifdef TMC2130_TRACE
	tmc2130_trace = uart1io;
#endif


	spi_init();
	adc_init();
	tmc2130_init();
	timer0_init();


	// set OCR0A register (125 means 0.5ms after OVF interrupt)
	OCR0A = 125;
	// enable OCR0A interrupt
	TIMSK0 |= (1 << OCIE0A);

//	st4_setup_axis(0, 100, 100, 10000, 250, 250); // res=100ustep/mm, sr0=1mm/s, srm=100mm/s, acc=?mm/s^2, dec=?mm/s^2
//	st4_setup_axis(1, 100, 100, 10000, 250, 250); // res=100ustep/mm, sr0=1mm/s, srm=100mm/s, acc=?mm/s^2, dec=?mm/s^2
//	st4_setup_axis(2, 100, 100, 10000, 250, 250); // res=100ustep/mm, sr0=1mm/s, srm=100mm/s, acc=?mm/s^2, dec=?mm/s^2

	st4_setup_timer();

}

uint16_t einsy_get_adc(uint8_t index)
{
	return adc_value[index];
}


void adc_ready(uint8_t index)
{
	adc_value[index] = adc_val[index];
}

uint8_t adc_seq2idx(uint8_t index)
{
	return index;
}

// Timer0 COMPA - every 1ms
ISR(TIMER0_COMPA_vect)
{
	adc_cycle();
}

