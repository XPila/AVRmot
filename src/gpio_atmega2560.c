// gpio_atmega2560.h

#include <avr/io.h>
#include "gpio_atmega2560.h"


uint8_t gpio_get(uint8_t gpio)
{
	uint8_t v = 0;
	switch (gpio >> 4)
	{
	case 0: v = PINA; break;
	case 1: v = PINB; break;
	case 2: v = PINC; break;
	case 3: v = PIND; break;
	case 4: v = PINE; break;
	case 5: v = PINF; break;
	case 6: v = PING; break;
	case 7: v = PINH; break;
	case 8: v = PINJ; break;
	case 9: v = PINK; break;
	case 10: v = PINL; break;
	}
	return (v & (1 << (gpio & 0x07)))?1:0;
}

void gpio_set(uint8_t gpio, uint8_t val)
{
	uint8_t v = 0;
	switch (gpio >> 4)
	{
	case 0: v = PORTA; break;
	case 1: v = PORTB; break;
	case 2: v = PORTC; break;
	case 3: v = PORTD; break;
	case 4: v = PORTE; break;
	case 5: v = PORTF; break;
	case 6: v = PORTG; break;
	case 7: v = PORTH; break;
	case 8: v = PORTJ; break;
	case 9: v = PORTK; break;
	case 10: v = PORTL; break;
	}
	if (val) v |= (1 << (gpio & 0x07)); else v &= ~(1 << (gpio & 0x07));
	switch (gpio >> 4)
	{
	case 0: PORTA = v; break;
	case 1: PORTB = v; break;
	case 2: PORTC = v; break;
	case 3: PORTD = v; break;
	case 4: PORTE = v; break;
	case 5: PORTF = v; break;
	case 6: PORTG = v; break;
	case 7: PORTH = v; break;
	case 8: PORTJ = v; break;
	case 9: PORTK = v; break;
	case 10: PORTL = v; break;
	}
}

void gpio_cfg(uint8_t gpio, uint8_t cfg)
{
	uint8_t v = 0;
	switch (gpio >> 4)
	{
	case 0: v = DDRA; break;
	case 1: v = DDRB; break;
	case 2: v = DDRC; break;
	case 3: v = DDRD; break;
	case 4: v = DDRE; break;
	case 5: v = DDRF; break;
	case 6: v = DDRG; break;
	case 7: v = DDRH; break;
	case 8: v = DDRJ; break;
	case 9: v = DDRK; break;
	case 10: v = DDRL; break;
	}
	if (cfg) v |= (1 << (gpio & 0x07)); else v &= ~(1 << (gpio & 0x07));
	switch (gpio >> 4)
	{
	case 0: DDRA = v; break;
	case 1: DDRB = v; break;
	case 2: DDRC = v; break;
	case 3: DDRD = v; break;
	case 4: DDRE = v; break;
	case 5: DDRF = v; break;
	case 6: DDRG = v; break;
	case 7: DDRH = v; break;
	case 8: DDRJ = v; break;
	case 9: DDRK = v; break;
	case 10: DDRL = v; break;
	}
}
