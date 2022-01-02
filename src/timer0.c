//timer0.c
#include "timer0.h"
#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint32_t timer0_ms; // 32bit milisecond counter (overflow in 49.71 days)

void timer0_init(void)
{
	timer0_ms = 0; // initialize milisecond counter to zero
	//normal mode, source = fclk/64
	TCCR0A = 0x00; //COM_A-B=00, WGM_0-1=00
	TCCR0B = TIMER0_PRESCALER; //WGM_2=0, CS_0-2=011
	TCNT0 = (256 - TIMER0_CYC_1MS); // initialize counter - overflow in 1ms
	TIMSK0 |= (1 << TOIE0); // enable timer overflow interrupt
}

uint32_t timer0_us(void)
{
	uint8_t _sreg = SREG;
	uint8_t tcnt;
	uint32_t ms;
	cli();
	tcnt = TCNT0;
	ms = timer0_ms;
	SREG = _sreg;
	return (1000 * ms) + (4 * (tcnt - (256 - TIMER0_CYC_1MS)));
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 += (256 - TIMER0_CYC_1MS);
	sei();
	timer0_ms++;
}
