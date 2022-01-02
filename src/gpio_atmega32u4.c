// gpio_atmega32u4.h

#include <avr/io.h>
#include "gpio_atmega32u4.h"


uint8_t gpio_get(uint8_t gpio)
{
	uint8_t v = 0;
	switch (gpio >> 4)
	{
	case 1: v = PINB; break;
	case 2: v = PINC; break;
	case 3: v = PIND; break;
	case 4: v = PINE; break;
	case 5: v = PINF; break;
	}
	return (v & (1 << (gpio & 0x07)))?1:0;
}

void gpio_set(uint8_t gpio, uint8_t val)
{
	uint8_t v = 0;
	switch (gpio >> 4)
	{
	case 1: v = PORTB; break;
	case 2: v = PORTC; break;
	case 3: v = PORTD; break;
	case 4: v = PORTE; break;
	case 5: v = PORTF; break;
	}
	if (val) v |= (1 << (gpio & 0x07)); else v &= ~(1 << (gpio & 0x07));
	switch (gpio >> 4)
	{
	case 1: PORTB = v; break;
	case 2: PORTC = v; break;
	case 3: PORTD = v; break;
	case 4: PORTE = v; break;
	case 5: PORTF = v; break;
	}
}

void gpio_cfg(uint8_t gpio, uint8_t cfg)
{
	uint8_t v = 0;
	switch (gpio >> 4)
	{
	case 1: v = DDRB; break;
	case 2: v = DDRC; break;
	case 3: v = DDRD; break;
	case 4: v = DDRE; break;
	case 5: v = DDRF; break;
	}
	if (cfg) v |= (1 << (gpio & 0x07)); else v &= ~(1 << (gpio & 0x07));
	switch (gpio >> 4)
	{
	case 1: DDRB = v; break;
	case 2: DDRC = v; break;
	case 3: DDRD = v; break;
	case 4: DDRE = v; break;
	case 5: DDRF = v; break;
	}
}
